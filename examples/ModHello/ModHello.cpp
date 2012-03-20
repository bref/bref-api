/**
 * \file   ModHello.cpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Fri Mar 16 15:26:37 2012
 *
 * \brief  ModHello definition.
 *
 */

#include "ModHello.h"
#include "bref/ScopedLogger.h"
#include "bref/detail/BrefDLL.h"

#include <utility>

const std::string ModHello::Name           = "mod_hello";
const std::string ModHello::Description    = "Retourne un body avec \"Hello world\".";
const float       ModHello::ModulePriority = 0.f; // Low priority

extern "C" BREF_DLL
bref::IModule *loadModule(bref::ILogger *logger)
{
  LOG_INFO(logger) << "Load module \"" << ModHello::Name << "\"";
  return new ModHello();
}

ModHello::ModHello()
  : version_(0, 1),
    apiVersion_(0, 1)
{ }

ModHello::~ModHello()
{ }

const std::string & ModHello::name() const
{
  return ModHello::Name;
}

const std::string & ModHello::description() const
{
  return ModHello::Description;
}

const bref::Version & ModHello::version() const
{
  return version_;
}

const bref::Version & ModHello::minimumApiVersion() const
{
  return apiVersion_;
}

/*
  La mémoire allouée dans une librairies dynamique par la méthode
  loadModule(), doit être nettoyée dans cette même librairie
  dynamique.
*/
void ModHello::dispose()
{
  delete this;
}

namespace {

/*
  Un vrai module (utile) utiliserais au moins la "request" mais ce
  n'est pas l'objectif de cette demo.
*/
bref::Pipeline::IContentRequestHandler *
modHelloGenerator(const bref::Environment & /* environment */,
                  const bref::HttpRequest & /* request */,
                  bref::HttpResponse & /* response */,
                  bref::FdType & /* fd */)
{
  return new ModHelloRequestHandler();
}

} // ! unnamed namespace

/*
  Enregistrement des points de hook dans la pipeline.
*/
void ModHello::registerHooks(bref::Pipeline & pipeline)
{
  pipeline.contentHooks.push_back(std::make_pair(&modHelloGenerator,
                                                 ModHello::ModulePriority));
}

ModHelloRequestHandler::ModHelloRequestHandler()
{ }

ModHelloRequestHandler::~ModHelloRequestHandler()
{ }

bool ModHelloRequestHandler::inContent(bref::HttpResponse & response,
                                       const bref::Buffer & inBuffer)
{
  // Non utilisée ici, on s'en fiche de ce que l'utilisateur nous envoie...
  // Du coup on retourne "true" pour ne pas être rappelé.
  return true;
}

bool ModHelloRequestHandler::outContent(bref::HttpResponse & response,
                                        bref::Buffer &       outBuffer)
{
  // HTTP/1.1
  response.setVersion(bref::Version(1, 1));

  // On retourne 200 "OK"
  response.setStatus(bref::status_codes::OK);
  response.setReason("OK");

  // On insère notre body dans le buffer de sortie
  const std::string hw = "Hello World !";
  outBuffer.insert(outBuffer.end(), hw.begin(), hw.end());

  // On connait la taille de notre body, on l'insert dans le header de réponse
  response["Content-Length"] = bref::BrefValue(static_cast<int>(hw.size()));

  // On n'a pas besoin d'être rapellé vu qu'on a retourné l'ensemble
  // de notre corps.
  return true;
}

void ModHelloRequestHandler::dispose()
{
  delete this;
}
