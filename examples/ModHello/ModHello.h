/**
 * \file   ModHello.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Fri Mar 16 15:23:26 2012
 *
 * \brief  ModHello class declaration.
 *
 */

#ifndef BREF_API_EXAMPLES_MODHELLO_MODHELLO_H_
#define BREF_API_EXAMPLES_MODHELLO_MODHELLO_H_

#include "bref/IModule.h"

class ModHello : public bref::IModule
{
public:
  static const std::string Name;
  static const std::string Description;
  static const float       ModulePriority;

private:
  bref::Version version_;
  bref::Version apiVersion_;

public:
  ModHello();
  virtual ~ModHello();
  virtual const std::string & name() const;
  virtual const std::string & description() const;
  virtual const bref::Version & version() const;
  virtual const bref::Version & minimumApiVersion() const;
  virtual void dispose();
  virtual void registerHooks(bref::Pipeline & pipeline);
};

/*
  Request handler for the ModHello module.
*/
struct ModHelloRequestHandler : public bref::Pipeline::IContentRequestHandler
{
  ModHelloRequestHandler();
  virtual ~ModHelloRequestHandler();
  virtual bool inContent(bref::HttpResponse & response, const bref::Buffer & inBuffer);
  virtual bool outContent(bref::HttpResponse & response, bref::Buffer & outBuffer);
  virtual void dispose();
};

#endif /* !BREF_API_EXAMPLES_MODHELLO_MODHELLO_H_ */
