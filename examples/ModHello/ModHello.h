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

#include "bref/AModule.h"

class ModHello : public bref::AModule
{
private:
  static const float       ModulePriority;

public:
  ModHello();
  virtual ~ModHello();
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
