/**
 * \file   ModCGI.cpp
 * \author Rannou Pierre <pierre.rannou@epitech.eu>
 * \date   Mon Mar 19 08:59:45 2012
 *
 * \brief  ModCGI class definition.
 *
 */

#include     <boost/algorithm/string/predicate.hpp>
#include     <utility>

#include     <sys/ioctl.h>
#include     <unistd.h>
#include     <errno.h>
#include     <stdio.h>

#include     "bref/AModule.h"
#include     "bref/ScopedLogger.h"
#include     "bref/IConfHelper.h"

//
// Prototype du [handler](documentation-example-ModCGI.html#exemple-d'un-début-d'implémentation-d'un-contenthooks).
//
bref::Pipeline::IContentRequestHandler *
ModCGIGenerator(const bref::Environment &  env,
                const bref::HttpRequest &  req,
                bref::HttpResponse &       response,
                bref::FdType &             fd);

// == Initialisation du module ==

// **Pour savoir comment implémenter un module avec l'API Bref, regardez le code
// documenté du [ModRewrite](documentation-example-ModRewrite.html#code)**.
//
// La documentation du *ModCGI* se porte essentiellement sur l'implémentation d'un
// module se liant sur le hook *contentHooks* en utilisant le système d'évènements du
// serveur.
//
// Pour un module plus simple, générant du contenu statique, nous vous conseillons de
// regarder le [ModHello](https://github.com/bref/bref-api/tree/master/examples/ModHello)
// disponible sur le dépôt Github de l'API Bref.

class ModCGI : public bref::AModule
{
private:
  const float               priority_;

public:
  ModCGI()
    : AModule("mod_cgi", "A CGI module able to execute Ruby", bref::Version(0, 2), bref::Version(0, 3))
    // Priorité haute : on génère du contenu dynamique avant d'autres potentiels
    // modules qui retourneraient le script comme du contenu statique.
    , priority_(1.f)
  { }

  virtual void dispose()
  {
    delete this;
  }

  virtual ~ModCGI()
  { }

  void registerHooks(bref::Pipeline & pipeline)
  {
     // Le hook est enristré en tant que "contentHooks" sur la pipeline
     pipeline.contentHooks.push_back(std::make_pair(&ModCGIGenerator,
                                                    priority_));
  }
};

// == Callbacks ==

struct ModCGIRequestHandler : public bref::Pipeline::IContentRequestHandler
{
    ModCGIRequestHandler(bref::FdType & fdIn,
                         bref::FdType & fdOut)
    : fdIn_(fdIn), fdOut_(fdOut)
    { }

    ~ModCGIRequestHandler()
    { }

    bref::FdType & fdIn_;
    bref::FdType & fdOut_;

    // === Callback in ===

    // **Cette fonction sera exécutée par le serveur avec un morceau de buffer du body de la requête
    // HTTP. Un appel avec un buffer vide entrainera la fermeture du pipe de l'entree standard
    // du processus CGI.**
    bool 	inContent(bref::HttpResponse & response,
                      const bref::Buffer & inBuffer)
    {
        // Plus de données à écrire.
        if (inBuffer.size() == 0)
        {
            close(fdIn_);
            return true;
        }

        // On écrit le buffer du body de la requète HTTP sur le pipe de l'entrée standard du processus CGI.
        ::write(fdIn_, inBuffer.data(), inBuffer.size());
        return false;
    }

    // === Callback out ===

    // **Cette fonction sera appelée lorsqu'il aura de l'activité sur le fd enregistré par le hook du module
    // par votre système d'évènements (kqueue / epoll / WSApoll / select / ...).**
    bool 	outContent(bref::HttpResponse & response,
                       bref::Buffer &       outBuffer)
    {
        int len = 0;

        // On récupère la taille disponible à lire.
        if (::ioctl(fdOut_, FIONREAD, len) < 0)
            return true;

        char *buff = new char[len];
        // On lit les données sur la sortie standard / d'erreur du processus CGI.
        if (::read(fdOut_, buff, len) < 0)
        {
            delete[] buff;
            return true;
        }

        // On insert les données dans notre buffer de sortie.
        outBuffer.insert(outBuffer.end(), buff, buff + len);
        delete[] buff;
        return false;
    }

    void dispose()
    {
        close(fdOut_);
        delete this;
    }
};

// === Exemple d'un début d'implémentation d'un contentHooks ===

// Cet exemple est un **début d'implémentation incomplet** d'un **potentiel** module CGI
// Il sert uniquement à montrer le fonctionnement des modules de l'API Bref, notamment
// l'utilisation d'un fd comme évènement déclancheur d'une callback.
// En conséquence l'exemple, incomplet (pas d'environnement, pas de wait des process, etc.),
// ne contient aucune abstraction et est prévu pour tourner uniquement sous UNIX.
bref::Pipeline::IContentRequestHandler *
ModCGIGenerator(const bref::Environment &  env,
                const bref::HttpRequest &  req,
                bref::HttpResponse &       response,
                bref::FdType &             fd)
{
    // On limite notre exécution CGI aux scripts ruby.
    if (!boost::algorithm::ends_with(req.getUri(), ".rb"))
        return NULL;

    int fdOut[2], fdIn[2];
    pid_t pid;

    // On crée deux pipe pour le corps de la requête et la réponse.
    if (pipe(fdOut) == -1 || pipe(fdIn) == -1 || (pid = fork()) == -1)
    {
        // Si un appel système échoue, on log l'erreur et retourne une 500.
        LOG_ERROR(env.logger) << "[ModCGI] " << strerror(errno);
        response.setStatus(bref::status_codes::InternalServerError);
        return NULL;
    }

    if (pid == 0)
    {
        // On dup l'entrée standard et les sorties standard / d'erreur du processus CGI.
        if (close(fdIn[0]) == -1 || close(fdOut[1]) == -1 ||
            dup2(0, fdIn[1]) == -1 ||
            dup2(1, fdOut[0]) == -1 || dup2(2, fdOut[0]) == -1)
        {
            LOG_ERROR(env.logger) << "[ModCGI] " << strerror(errno);
            exit(1);
        }

        // On récupère le répertoire de base du serveur / virtualhost. Cette fonction
        // [findValue](http://bref.github.com/documentation-api.html#confhelper)
        // retourne la valeur la plus pertinente en fonction de la requête.
        std::string DocumentRoot = env.serverConfigHelper.findValue("DocumentRoot", req).asString();
        // Le chemin absolu du script : DocumentRoot + URI,
        // par exemple : "/var/www" + "/script.rb" = /var/www/script.rb
        std::string script = DocumentRoot + req.getUri();

        // On exécute le script CGI.
        execlp(script.c_str(), script.c_str(), NULL);

        // Si l'`execlp` échoue.
        LOG_ERROR(env.logger) << "[ModCGI] " << strerror(errno);
        exit(1);
    }

    if (close(fdIn[1]) == -1 || close(fdOut[0]) == -1)
    {
        LOG_ERROR(env.logger) << "[ModCGI] " << strerror(errno);
        response.setStatus(bref::status_codes::InternalServerError);
        return NULL;
    }

    // On définit le fd pour qu'il soit utilisé dans le système d'évènements du serveur.
    fd = fdOut[1];

    // On retoure un handler.
    return new ModCGIRequestHandler(fdIn[0], fdOut[1]);
}

// == Enregistrement du module ==

// Pour plus d'informations, regarde le module [ModRewrite](documentation-example-ModRewrite.html#enregistrement-du-module).
extern "C" BREF_DLL
bref::AModule *loadModule(bref::ILogger *logger,
                          const bref::ServerConfig &,
                          const bref::IConfHelper &)
{
    LOG_INFO(logger) << "Load CGI module";
    return new ModCGI();
}
