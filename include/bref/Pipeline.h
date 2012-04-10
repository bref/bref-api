/**
 * \file   Pipeline.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sat Mar 10 23:29:24 2012
 *
 * \brief  Pipeline declaration.
 *
 */

#ifndef BREF_API_PIPELINE_H
#define BREF_API_PIPELINE_H

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
# include <windows.h>
#endif
#include "Function.hpp"
#include "IConfHelper.h"
#include "BrefValue.h"
#include "ILogger.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "IpAddress.h"
#include "Buffer.h"
#include "IDisposable.h"

#include <list>
#include <vector>
#include <utility>

namespace bref {
/**
 * \defgroup Pipeline Pipeline
 *
 * \brief The pipeline contains all the hooks points available for the
 *        bref API.
 *
 * When a module is initialized by the method
 * \c AModule#registerHooks(Pipeline &) it initializes its hook points
 * in the Pipeline.
 *
 * This page describes how the Pipeline class works and how it interacts
 * with the different hook points.
 *
 * Vocabulary:
 * - \e Hook:
 *
 *   Hooks are RequestHandlers (see below) generators for a given set
 *   of arguments. If -- given the argument(s) it received -- a hook
 *   decides not to handle a request, a null handler is generated.
 *
 *   Examples of hooks are: Pipeline::PostParsingHook,
 *   Pipeline::OnReceiveHook, ...
 *
 *   In term of code, a hook is a Function object that returns another
 *   Function object. It's often constructed with an Environment and an
 *   HttpRequest.
 *
 * - \e RequestHandler:
 *
 *   A request handler (e.g: Pipeline::ConnectionRequestHandler,
 *   Pipeline::OnReceiveRequestHandler) is the function object, returned
 *   by the hook, that will handle a request. It should be called with
 *   the arguments provided by its corresponding hook point
 *   (e.g.: the Pipeline::ConnectionRequestHandler takes an HttpResponse
 *   and an IpAddress).
 *
 *   The majority of the request handlers takes an HttpResponse as
 *   reference, they can fill its header. When a response is finished,
 *   the upstream or bridge elements of the pipeline will directly go
 *   to the downstream part.
 *
 * - \e Priority
 *
 *   Used to represent the precedence of one request handler over
 *   another, when placed on the same hook point.
 *   Depending on the hook point, either all the request handlers will be
 *   called, starting with the highest priority, or only the most important
 *   one will be called.
 *
 *   The priority is a floating number between 0.0 and 1.0.
 *   Modules should try to follow the following convention (but the value
 *   can be adapted):
 *   - Low Priority:    0.0
 *   - Normal Priority: 0.5
 *   - High Priority:   1.0
 *
 * Example for a CGI module:
 *
 * 1. Register a Pipeline::ContentHook with a high priority (1.0)
 *    that returns a valid Pipeline::ContentRequestHandler if it detects
 *    a request asking a PHP file (by checking the .php extension for
 *    example and some configuration variables), otherwise it should
 *    return an empty handler.
 *
 * 2. Create the Pipeline::ContentRequestHandler that creates a
 *    process when it's constructed, and has a function operator()
 *    that takes each chunk of the request body and send them to the
 *    CGI process, and fill a buffer in return.
 *
 * @{
 *
 */

/**
 * \brief The value used for the configuration settings of the server.
 *
 * A typedef is used here for a better readability.
 */
typedef BrefValue ServerConfig;

/**
 * \brief Define fd type (system dependant : fd on UNIX systems,
 *        HANDLE for Windows systems) for ContentHook.
 * \sa    ContentHook
 */
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
  typedef HANDLE FdType;
#else
  typedef int FdType;
#endif

/**
 * \brief This structure defines the environment of a request.
 *
 * The environment of a request contains a logger and the server
 * configuration.
 *
 * We choose to do this because an implementation is free to handle
 * different ILogger (with different output file) for each virtual
 * host for example.
 */
struct Environment
{
  const ServerConfig &  serverConfig; /**< the server configuration */
  const IConfHelper &   serverConfigHelper; /**< the helper on server configuration */
  ILogger              *logger;       /**< the logger for a given context */

  /**
   * \brief This structure contains client informations
   */
  struct Client
  {
    IpAddress  Ip;
    short      Port;
    SocketType Socket;
  }                     client; /**< The client description */

  Environment(const ServerConfig &  theServerConfig,
              const IConfHelper &   theServerConfigHelper,
              ILogger              *theLogger,
              Client                theClient)
    : serverConfig(theServerConfig)
    , serverConfigHelper(theServerConfigHelper)
    , logger(theLogger)
    , client(theClient)
  { }
};

/**
 * \brief Where the AModule classes register their hooks.
 *
 * See the \ref Pipeline "Pipeline" page for more information.
 */
struct Pipeline
{
  /**
   * \defgroup Gate Gate
   * \ingroup Pipeline
   *
   * \brief Connections, data reception and sending events handling.
   *
   * The gate handles the communication with the outside world. Network
   * events like:
   *
   * - a connection
   * - a notification (by select() for example) saying a socket got
   *   data to read
   * - when we have to send data to the client.
   *
   * @{
   */

  /**
   * \brief A hook for the connection of a client.
   *
   * \param[out] response
   *          Can be filled with the header of the response and a
   *          status code.
   * \param[in] environment
   *          The environment of the request.
   *
   * \retval true
   *    If the connection is accepted.
   * \retval false
   *    If the connection is refused and the socket should be closed
   *    by the server.
   *
   * \sa connectionHooks, ConnectionHook
   */
  typedef Function<bool (HttpResponse & response,
                         const Environment & environment)> ConnectionRequestHandler;

  /**
   * \brief Generate a Pipeline::ConnectionRequestHandler.
   *
   * \param[in] environment
   *            The environment of the request.
   *
   * \return An empty Pipeline::ConnectionRequestHandler when the
   *         module can not process the request, otherwise a
   *         valid handler that will be called upon connection
   *         of a client.
   *
   * \sa connectionHooks, ConnectionRequestHandler
   */
  typedef Function<ConnectionRequestHandler (const Environment & environment)> ConnectionHook;

  /**
   * \brief List of connection hooks.
   *
   * Connection hooks should be called by the ZIA server upon
   * the connection of a client.
   *
   * This hook point can be used for a black listing module for
   * example.
   *
   * \sa ConnectionHook, ConnectionRequestHandler
   */
  std::list<std::pair<ConnectionHook, float> > connectionHooks;

  /**
   * \brief The handler to call when there is pending data on a
   *        socket.
   *
   * The server should call this handler when the socket received some
   * "read event" on the fd, with a function like \c select().
   *
   * \param[in] socket
   *            The socket with data available.
   * \param[out] buffer
   *            The buffer where data will be stored.
   *
   * \retval true
   *    If everything went fine, the server can pursue the request.
   * \retval false
   *    When an unrecoverable error happens, the server will take care
   *    to remove the socket.
   *
   * \sa onReceiveHooks, OnReceiveHook
   */
  typedef Function<bool (SocketType socket,
                         Buffer &   buffer)> OnReceiveRequestHandler;

  /**
   * \brief Generate a Pipeline::OnReceiveRequestHandler.
   *
   * \param[in] environment
   *            The environment of the request.
   *
   * \return An empty handler when the module can not process the
   *         request, otherwise a valid handler that will be called upon
   *         reception of data.
   *
   * \sa onReceiveHooks, OnReceiveRequestHandler
   */
  typedef Function<OnReceiveRequestHandler (const Environment & environment)> OnReceiveHook;

  /**
   * \brief List of received hooks.
   *
   * OnReceiveHook should be called by the ZIA server when a read
   * event is received on a socket (with a select() or similar a
   * methods).
   *
   * This hook point can be used for a reading data on the socket. A
   * SLL module seems to be a good example.
   *
   * \sa OnReceiveHook, OnReceiveRequestHandler
   */
  std::list<std::pair<OnReceiveHook, float> > onReceiveHooks;

  /**
   * \brief A hook when data need to be sent.
   *
   * \param[in] socket
   *            The socket where buffer need to be sent.
   * \param[in] buffer
   *            The data to send.
   *
   * \retval true
   *    If everything went fine.
   * \retval false
   *    If an error occured. The server should close the socket.
   *
   * \sa onSendHooks, OnSendHook
   */
  typedef Function<bool (SocketType     socket,
                         const Buffer & buffer)> OnSendRequestHandler;

  /**
   * \brief Generate a Pipeline::OnSendRequestHandler.
   *
   * \param[in] environment
   *            The environment of the request.
   *
   * \return An empty Pipeline::OnSendRequestHandler when the
   *         module can not process the request, otherwise a
   *         valid handler that will be called when sending data.
   *
   * \sa onSendHooks, OnSendRequestHandler
   */
  typedef Function<OnSendRequestHandler (const Environment & environment)> OnSendHook;

  /**
   * \brief List of hooks to run when data needs to be sent.
   *
   * The server should call these hooks at the end of the Pipeline, when
   * a response should be sent to the client.
   *
   * \sa OnSendRequestHandler, onSendHook
   */
  std::list<std::pair<OnSendHook, float> > onSendHooks;

  /** @} */

  /**
   * \defgroup Upstream Upstream
   * \ingroup Pipeline
   *
   * \brief Represents the pre-processing of a request.
   *
   * The upstream handles each stage of a request before the
   * \ref Bridge "Bridge". It performs the parsing of the request
   * and provides a few hooks before and after this step.
   *
   * @{
   */

  /**
   * \brief A hook called upon reading data on the socket.
   *
   * This hook is able to transform data received by the socket
   * before it is sent to the parser.
   *
   * \param[out] response
   *            Can be filled with the header of the response and a
   *            status code.
   * \param[in] inBuffer
   *            The buffer containing the data received on the socket.
   * \param[out] outBuffer
   *            The buffer containing the data to send to the parser.
   *
   * \sa postReceiveHooks, PostReceiveHook
   */
  typedef Function<void (HttpResponse & response,
                         const Buffer & inBuffer,
                         Buffer &       outBuffer)> PostReceiveRequestHandler;

  /**
   * \brief Generate a Pipeline::PostRequestHandler.
   *
   * \param[in] environment
   *            The environment of the request.
   *
   * \return An empty Pipeline::PostRequestHandler if the hook can not
   *         process the request, a valid handler otherwise.
   *
   * \sa postReceiveHooks, PostReceiveRequestHandler
   */
  typedef Function<PostReceiveRequestHandler (const Environment & environment)> PostReceiveHook;

  /**
   * \brief List of post-read hooks.
   *
   * Post-read hooks should be called by the ZIA server after the
   * Pipeline::OnReceiveRequestHandler handlers have generated some
   * data, and before \c parsingHooks are called.
   *
   * \sa PostReceiveHook, PostReceiveRequestHandler
   */
  std::list<std::pair<PostReceiveHook, float> > postReceiveHooks;

  /**
   * \brief The handler called to generate an HttpRequest.
   *
   * It parses raw data received on the socket and converts it into a
   * structured HttpRequest.
   *
   * \param[out] response
   *            Can be filled with the header of the response and a
   *            status code.
   * \param[in] buff
   *            A chunk of raw data.
   * \param[out] request
   *            The HttpRequest to fill with the content of the request.
   *
   * \return A pointer to the consumed part of the buffer. If the
   *         parser finds the "\r\n\r\n" sequence in the middle of the
   *         buffer it will return an iterator to the character after
   *         the last '\n', otherwise an iterator to the beginning of
   *         buffer should be returned.
   *
   * \retval buff.begin()
   *    If the parser has not finished the parsing yet.
   *
   * \sa parsingHooks, ParsingHook
   */
  typedef Function<Buffer::const_iterator (HttpResponse & response,
                                           const Buffer & buff,
                                           HttpRequest &  request)> ParsingRequestHandler;

  /**
   * \brief Generate a Pipeline::ParsingRequestHandler.
   *
   * \return An empty Pipeline::ParsingRequestHandler when the hook
   *         can not process the request, a valid handler otherwise.
   *
   * \sa parsingHooks, ParsingRequestHandler
   */
  typedef Function<ParsingRequestHandler (const Environment &)> ParsingHook;

  /**
   * \brief List of parsing hooks.
   *
   * Only one parser module should return a valid request handler for a
   * given request. If multiple handlers are provided, the one with the
   * highest priority will be used.
   *
   * \sa ParsingHook, ParsingRequestHandler
   */
  std::list<std::pair<ParsingHook, float> > parsingHooks;

  /**
   * \brief Handler called once the request is parsed.
   *
   * This hook point can be used for an URL rewrite module for example.
   *
   * \param[out] response
   *            Can be filled with the response header and a status code.
   *
   * \sa postParsingHooks, PostParsingHook
   */
  typedef Function<void (HttpResponse & response)> PostParsingRequestHandler;

  /**
   * \brief Generate a Pipeline::PostParsingRequestHandler.
   *
   * \param[in] environment
   *            The environment of the request.
   * \param[out] httpRequest
   *            The HttpRequest to fill with the request header.
   *
   * \return An empty Pipeline::PostParsingRequestHandler if the
   *         hook can not process the request, otherwise a valid
   *         handler.
   *
   * \sa postParsingHooks, PostParsingRequestHandler
   */
  typedef Function<PostParsingRequestHandler (const Environment & environment,
                                              HttpRequest &       httpRequest,
                                              HttpResponse &      response)> PostParsingHook;

  /**
   * \brief List of post-parsing hooks.
   *
   * The server should call these hooks after the parsing step.
   *
   * \sa PostParsingRequestHandler, PostParsingHook
   */
  std::list<std::pair<PostParsingHook, float> > postParsingHooks;

  /** @} */

  /**
   * \defgroup Bridge Bridge
   * \ingroup Pipeline
   *
   * \brief Contains hooks to run for content generation.
   *
   * This is where modules like CGI, static pages, directory listing,
   * ... should be plugged.
   *
   * @{
   */

  /**
   * \brief Handler called to execute a request.
   *
   * This handlers take an optional input from the body of the request
   * and generates a response body.
   *
   */
  class IContentRequestHandler : public IDisposable
  {
  public:
    /**
     * \brief Callback for request body content
     *
     * \param [out] response
     *              Where the status code is filled.
     * \param [in] inBuffer
     *             A chunk of the request body.
     *
     * \retval true
     *    If the processing is finished.
     * \retval false
     *    If the processing is not finished.
     *
     * \sa contentHooks, ContentHook
     */
    virtual bool inContent(HttpResponse & response, const Buffer & inBuffer) = 0;

    /**
     * \brief Callback for response body content
     *
     * \param [out] response
     *              Where the status code is filled.
     * \param [in] inBuffer
     *             A buffer containing the generated content.
     *
     * \retval true
     *    If the processing is finished.
     * \retval false
     *    If the processing is not finished.
     *
     * \sa contentHooks, ContentHook
     */
    virtual bool outContent(HttpResponse & response, Buffer & outBuffer) = 0;

  protected:
    /**
     * \brief Virtual destructor
     *
     * The \c delete() operator should not be called on the IContentRequestHandler
     * instance. Please refer to the \c dispose() method.
     *
     * \note The destructor is protected in order to disable the call to
     *       the \c delete() operator from the server code.
     *
     * \sa dispose()
     */
    virtual ~IContentRequestHandler() {}
  };

  /**
   * \brief Generate Pipeline::ContentRequestHandler.
   *
   * \param [out] fd If you set this param in your Hook, the server will
   *              register your fd in its event system (kqueue / epoll / select)
   *              and call back your handle on new activity.
   *
   * \warning Server has to set a default value (depending on the system, for
   *          example -1 on UNIX integer, NULL on Windows HANDLE) for fd,
   *          to detect that the hook provides a fd.
   *
   * \return A 0 pointer if the hook can not process the request, a valid pointer
   *         to Pipeline::IContentRequestHandler otherwise.
   *
   * \sa contentHooks, ContentRequestHandler
   */
  typedef Function<IContentRequestHandler *(const Environment & environment,
                                            const HttpRequest & request,
                                            HttpResponse &      response,
                                            FdType &            fd)> ContentHook;


  /**
   * \brief List of content hooks.
   *
   * Only one content hook can be used for a given request, the
   * content hook with the highest priority should be choosed by the
   * server.
   *
   * \sa ContentRequestHandler, ContentHook
   */
  std::list<std::pair<ContentHook, float> > contentHooks;

  /** @} */

  /**
   * \defgroup Downstream Downstream
   * \ingroup Pipeline
   *
   * \brief Handle all stages between the content generation and
   *        sending data back to the client.
   *
   * This section can apply post hook transfomation, like compression
   * on the request.
   *
   * @{
   */

  /**
   * \brief Hook to run after the content was generated by
   *        \c contentHooks.
   *
   * \param[out] reponse
   *            Where the status code is filled.
   * \param inBuffer
   *            A chunk of the request body.
   * \param outBuffer
   *            An chunk of output body.
   *
   * \sa postContentHooks, PostContentHook
   */
  typedef Function<void (HttpResponse & reponse,
                         const Buffer & inBuffer,
                         Buffer &       outBuffer)> PostContentRequestHandler;

  /**
   * \brief Generate Pipeline::PostContentRequestHandler.
   *
   * \return An empty Pipeline::PostContentRequestHandler if the
   *         hook can not process the request, a valid handler otherwise.
   *
   * \sa postContentHooks, PostContentRequestHandler
   */
  typedef Function<PostContentRequestHandler (const Environment & environment,
                                              const HttpRequest & request,
                                              HttpResponse &      response)> PostContentHook;

  /**
   * \brief A list of post-content hooks.
   *
   * This hooks can be chained, but the one with the highest
   * priority will be called first.
   *
   * \sa PostContentRequestHandler, PostContentHook
   */
  std::list<std::pair<PostContentHook, float> > postContentHooks;


  /**
   * \brief Handler to call after the postContentHooks are executed.
   *
   * This hook can be used for a compression module for example.
   *
   * \param[out] response
   *            Where the status code is filled.
   * \param[in] inBuffer
   *            A chunk of the body of the request.
   * \param[out] outBuffer
   *            An chunk of output body.
   *
   * \sa transformHooks, TransformHook
   */
  typedef Function<void (HttpResponse & response,
                         const Buffer & inBuffer,
                         Buffer &       outBuffer)> TransformRequestHandler;

  /**
   * \brief Generate Pipeline::TransformRequestHandler.
   *
   * \return An empty Pipeline::TransformRequestHandler when the
   *         hook can not process the request, a valid handler
   *         otherwise.
   *
   * \sa transformHooks, TransformRequestHandler
   */
  typedef Function<TransformRequestHandler (const Environment & environment,
                                            const HttpRequest & request,
                                            HttpResponse &      response)> TransformHook;

  /**
   * \brief List of transformation hooks.
   *
   * Transformation hooks are called handle in descending order of priority.
   *
   * \sa TransformRequestHandler, TransformHook
   */
  std::list<std::pair<TransformHook, float> > transformHooks;

  /**
   * \brief Handler called before sending data back to the client.
   *
   * \param[out] reponse
   *            Where the status code is filled.
   * \param inBuffer
   *            A chunk of the request body.
   * \param outBuffer
   *            An chunk of output body.
   *
   * \sa preSendHooks, PreSendHook
   */
  typedef Function<void (HttpResponse & response,
                         const Buffer & inBuffer,
                         Buffer &       outBuffer)> PreSendRequestHandler;

  /**
   * \brief Generate Pipeline::PreSendRequestHandler.
   *
   * \return An empty Pipeline::PreSendRequestHandler if the
   *         hook can not process the request, a valid handler otherwise.
   *
   * \sa preSendHooks, PreSendRequestHandler
   */
  typedef Function<PreSendRequestHandler (const Environment & environment,
                                          const HttpRequest & request,
                                          HttpResponse &      response)> PreSendHook;

  /**
   * \brief A list of pre-send hooks.
   *
   * These hooks can be chained, but only the first one, in order of
   * descending priority, will be called.
   *
   * \sa PreSendRequestHandler, PreSendHook
   */
  std::list<std::pair<PreSendHook, float> > preSendHooks;

  /** @} */

};

/** @} */

} // ! bref

#endif /* !BREF_API_PIPELINE_H */
