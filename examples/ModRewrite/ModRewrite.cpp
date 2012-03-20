// == Introduction ==

//
// Ce module est écrit dans un seul fichier, pas de séparations entre
// déclarations et définitions pour simplifier la démonstration. Il
// montre un usage basique de l'API Bref. Le module écrit est très
// simple dans son fonctionnement et vise surtout à expliquer les
// notions basiques de l'écriture d'un module.
//
// Cet exemple est présenté sous forme de *literate programming*,
// avec en parallèle le code et son explication. Il présente
// l'écriture d'un module simple d'[URL
// rewriting](http://en.wikipedia.org/wiki/Rewrite_engine).
//

// === Note préliminaire ===

//
// Vous pourrez constater que **nous n'avons pas choisi de faire un
// module qui s'écrit en 2 lignes**, mais plutôt d'essayer de faire
// quelque chose d'un peu plus **réaliste** en forçant la définition
// d'une version, d'une description, etc. Cela coûte 2 minutes de plus
// à écrire, mais permettra aux serveurs qui le désirent de fournir
// certaines fonctionnalitées un peu plus avancées dans la gestion des
// modules.
//

// == Code ==

//
// Chaque module doit hériter de l'interface
// [IModule](http://bref.github.com/doxygen/classbref_1_1_i_module.html).
//
#include "bref/IModule.h"

//
// Utile pour le l'utilisation de d'`ostream` pour le logging.
//
#include "bref/ScopedLogger.h"

//
// Prototype du handler pour la ré-écriture d'URL (voir:
// [[ModRewrite#code-de-rewriting]]).
//
bref::Pipeline::PostParsingRequestHandler
rewriteURLHook(const bref::Environment &, bref::HttpRequest &, bref::HttpResponse &);

//
// Nous déclarons ici une classe héritant de l'interface IModule,
// cette classe sera instanciée un peu plus bas dans la fonction
// [loadModule()](ModRewrite.cpp#enregistrement-du-module).
//
class ModRewrite : public bref::IModule
{
  //
  // Déclaration des variables internes au module
  //
private:
  const std::string   name_;
  const std::string   description_;
  const bref::Version version_;
  const bref::Version apiVersion_;
  const float         priority_;

public:
  //
  // Définition des variables.
  //
  ModRewrite()
    : name_("mod_rewrite")
    , description_("A simple URL-rewrite module")
    , version_(0, 1)
      //
      // Vous pouvez retrouver la dernière version de l'API
      // [ici](https://github.com/bref/bref-api/tags).
      //
    , apiVersion_(0, 2)
      //
      // Si on regarde dans le doxygen [la partie sur la
      // priorité](http://bref.github.com/doxygen/group___pipeline.html)
      // on mets ici une priorité normale. La ré-écriture d'URL ne
      // semble pas avoir besoin d'être fait en priorité par rapport à
      // un autre module.
      //
    , priority_(0.5)
  { }

  //
  // Définition des méthodes basiques donnant des informations sur le
  // module.
  //
  virtual const std::string &   name()              const {return name_;}
  virtual const std::string &   description()       const {return description_;}
  virtual const bref::Version & version()           const {return version_;}
  virtual const bref::Version & minimumApiVersion() const {return apiVersion_;}

  //

  //
  // Cette méthode permet de libérer un module alloué dans une
  // librarie dynamique. Lorsque l'allocation (`new`) a été effectuée
  // dans une librarie dynamique, il est nécessaire que la libération
  // de la mémoire (`delete`) y soit aussi effectuée. Comme expliqué
  // dans [le doxygen d'IModule](http://bref.github.com/doxygen/classbref_1_1_i_module.html)
  // cette méthode est souvent représentée par un body contenant
  // `delete this;`
  //
  virtual void dispose()
  {
    delete this;
  }

  virtual ~ModRewrite()
  { }

  // === Enregistrement du hook ===

  //
  // On souhaite faire un module d'URL rewrite, l'endroit parfait pour
  // faire ça, si on regarde [le schéma de la
  // Pipeline](./documentation-intro.html) est le
  // [PostParsingHook](./doxygen/group___upstream.html#ga5af7ca7c7f953a8f0f7292b0a71443f0)
  //
  virtual void registerHooks(bref::Pipeline & pipeline)
  {
    pipeline.postParsingHooks.push_back(std::make_pair(&rewriteURLHook,
                                                       priority_));
  }

};


// == Code de rewriting ==

//
// Ce module d'URL change l'extension d'un fichier dans l'url, à l'origine en
// `.html` en un fichier `.php`. Si l'on souhaite faire un vrai module
// d'URL rewrite il faudrait probablement se baser sur la
// configuration présente dans l'`Environment`. Et il faudrait aussi
// faire attention à ne pas prendre les paramètres d'un GET ou autre,
// ce n'est pas l'objectif de cet exemple.
//
bref::Pipeline::PostParsingRequestHandler
rewriteURLHook(const bref::Environment & /* environment */,
               bref::HttpRequest &          httpRequest,
               bref::HttpResponse &      /* response */)
{
  const std::string & uri     = httpRequest.getUri();
  const std::size_t   extSize = sizeof ".html" - 1;

  //
  // Vérification de l'extension `.html`.
  //
  if (uri.length() >= extSize &&
      ! uri.compare(uri.length() - extSize, extSize, ".html")) {
    //
    // Suppression de l'extension `.html`.
    //
    std::string newUri(uri.begin(), uri.end() - extSize);

    //
    // Ajout de l'extension `.php`.
    //
    newUri += ".php";

    //
    // On modifie le header avec la nouvelle adresse.
    //
    httpRequest.setUri(newUri);
  }

  //
  // On a fait tout ce qu'on avait à faire, le body de la requête ne
  // nous intéresse pas, on retourne donc un handler vide.
  //
  return bref::Pipeline::PostParsingRequestHandler();
}

// == Enregistrement du module ==

//
// Cette fonction est la méthode d'entrée d'un module, elle permettra
// au serveur d'avoir une instance du module. Certains modules peuvent
// nécessiter l'accès à la configuration du server pour se loader,
// c'est pourquoi on envoie en paramètre un `ServerConfig` et
// `IConfHelper`.
//
extern "C" BREF_DLL
bref::IModule *loadModule(bref::ILogger *logger,
                          const bref::ServerConfig &,
                          const bref::IConfHelper &)
{
  //
  // Voir les macros de logging [sur le
  // doxygen](/doxygen/_scoped_logger_8h.html).
  //
  LOG_DEBUG(logger) << "Loading URL rewrite module";
  return new ModRewrite;
}
