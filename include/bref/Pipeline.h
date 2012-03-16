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

#include "Function.hpp"
#include "BrefValue.h"
#include "ILogger.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "IpAddress.h"
#include "Buffer.h"

#include <list>
#include <vector>
#include <utility>
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
# include <windows.h>
#endif

namespace bref {
/**
 * \defgroup Pipeline Pipeline
 *
 * \brief The pipeline contain all the hooks points available for the
 *        bref API.
 *
 * When a module is initialized by the method
 * \c IModule#registerHooks(Pipeline &) it initialize it's hook points in
 * the Pipeline.
 *
 * This page describe how the Pipeline class and how it interact with
 * the different hook points.
 *
 * Vocabulary:
 * - \e Hook:
 *
 *   Hooks are RequestHandlers (see below) generators for a given set
 *   of arguments. If -- given the argument it received -- a hook
 *   decide to not handle a request a null handler is generated.
 *
 *   Examples of hooks are: Pipeline::PostParsingHook,
 *   Pipeline::OnReceiveHook, ...
 *
 *   In term of codes, a hook is a Function object that return another
 *   Function object. It's often constructed by an Environment and an
 *   HttpRequest.
 *
 * - \e RequestHandler:
 *
 *   A request handler (i.e: Pipeline::ConnectionRequestHandler,
 *   Pipeline::OnReceiveRequestHandler) is the handler that handle.
 *
 *   It's the function object returned by the hook that will handle a
 *   request. It should be called with the arguments provided by this
 *   hook point (i.e: the Pipeline::ConnectionRequestHandler take a
 *   HttpResponse to fill and an IpAddress).
 *
 *   The majority of the request handler take an HttpResponse as
 *   reference, they can fill it's header and body. When a response is
 *   finished the upstream or bridge elements of the pipeline will
 *   directly go to the downstream part.
 *
 * - \e Priority
 *
 *   Use to represent the priority of one request handler on another,
 *   when placed on the same hook point. A request handler with higher
 *   priority will be used over one with a lower's priority.
 *
 *   The priority is a floating number. Modules should try to follow
 *   the following convention (but value can be adapted):
 *   - Low Priority:    0.0
 *   - Normal Priority: 0.5
 *   - High Priority:   1.0
 *
 * Example for a CGI module:
 *
 * 1. Register a Pipeline::ContentHook with a high priority (1.0)
 *    that return a valid Pipeline::ContentRequestHandler when it detect a
 *    request asking a PHP file (by checking the .php extension for example
 *    and some configuration variables), otherwise it should return an
 *    empty handler.
 *
 * 2. Create the Pipeline::ContentRequestHandler that create a
 *    process when it's constructed, and have a function operator()
 *    that take each chunk of the request body and send them to the
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
 * \brief This structure define the environment of a request.
 *
 * The environment of a request is currently a logger and the server
 * configuration.
 *
 * We choose to do this because an implementation is free to handle
 * different ILogger (with different output file) for each virtual
 * host for example.
 */
struct Environment
{
  const ServerConfig &  serverConfig; /**< the server configuration */
  ILogger              *logger;       /**< the logger for a given context */

  /**
   * \brief This structure contains client informations
   */
  struct Client
  {
    IpAddress  Ip;
    short      Port;
    SocketType Socket;
  };
};

/**
 * \brief Where the IModule classes register their hooks.
 *
 * See the \ref Pipeline "Pipeline" page for more information.
 */
struct Pipeline
{
  /**
   * \defgroup Gate Gate
   * \ingroup Pipeline
   *
   * \brief Connections, received and send events handling.
   *
   * The gate handle the communication with the outside world. Network
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
   *          Can be filled with the content of the response and a
   *          status code.
   * \param environment
   *            The environment of the request.
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
   * \param environment
   *            The environment of the request.
   *
   * \return An empty Pipeline::ConnectionRequestHandler when the
   *         module has nothing to do for the request, otherwise a
   *         valid handler that will be called to handle the
   *         connection of a client.
   *
   * \sa connectionHooks, ConnectionRequestHandler
   */
  typedef Function<ConnectionRequestHandler (const Environment & environment)> ConnectionHook;

  /**
   * \brief List of connection hooks.
   *
   * Connection hooks should be called by the ZIA server when it
   * received the connection of a client.
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
   *            The socket where data need to be read.
   * \param[out] buffer
   *            The buffer where data will be stored.
   *
   * \retval true
   *    If everything went fine the server can pursue the request.
   * \retval false
   *    When an unrecoverable error happen, the server will take care
   *    to remove the socket.
   *
   * \sa onReceiveHooks, OnReceiveHook
   */
  typedef Function<bool (SocketType socket,
                         Buffer &   buffer)> OnReceiveRequestHandler;

  /**
   * \brief Generate a Pipeline::OnReceiveRequestHandler.
   *
   * \param environment
   *
   * \return An empty handler when the module has nothing to do for the
   *         request, otherwise a valid handler that will be called to
   *         handle the reception of data.
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
   * \param environment
   *
   * \return An empty Pipeline::OnSendRequestHandler when the
   *         module has nothing to do for the request, otherwise a
   *         valid handler that will be called to handle the
   *         connection of a client.
   *
   * \sa onSendHooks, OnSendRequestHandler
   */
  typedef Function<OnSendRequestHandler (const Environment & environment)> OnSendHook;

  /**
   * \brief List of hooks to run when data needs to be sent.
   *
   * The server should call this hook at the en of the Pipeline, when
   * a response should be written to the client.
   *
   * \sa OnSendRequestHandler, onSendHook
   */
  std::list<std::pair<OnSendHook, float> > onSendHooks;

  /** @} */

  /**
   * \defgroup Upstream Upstream
   * \ingroup Pipeline
   *
   * \brief Represent the pre-processing of a request.
   *
   * The upstream handle each stage of a request before the
   * \ref Bridge "Bridge". This section handle the parsing of the
   * request and a few hooks before and after the parsing step.
   *
   * @{
   */

  /**
   * \brief A hook called after something was read on the socket.
   *
   * This hook is able to transform the data received by the socket
   * and transform before they are sent to the parser.
   *
   * \param response
   *            Can be filled with the content of the response and a
   *            status code.
   * \param[in] inBuffer
   *            The buffer of raw datas, received on the socket.
   * \param[out] outBuffer
   *            The content of the buffer to send to the parser.
   *
   * \sa postReadHooks, PostReadHook
   */
  typedef Function<void (HttpResponse & response,
                         const Buffer & inBuffer,
                         Buffer &       outBuffer)> PostReadRequestHandler;

  /**
   * \brief Generate a Pipeline::PostRequestHandler.
   *
   * \return An empty Pipeline::PostRequestHandler when the
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa postReadHooks, PostReadRequestHandler
   */
  typedef Function<PostReadRequestHandler (const Environment & environment)> PostReadHook;

  /**
   * \brief List of post-read hooks.
   *
   * Post-read hooks should be called by the ZIA server after the
   * Pipeline::OnReceiveRequestHandler handlers have generated some
   * data, and before \c parsingHooks are called.
   *
   * \sa PostReadHook, PostReadRequestHandler
   */
  std::list<std::pair<PostReadHook, float> > postReadHooks;

  /**
   * \brief The handler called to generate an HttpRequest.
   *
   * It parse raw datas from the socket and convert them into a
   * structured HttpRequest.
   *
   * \param[out] response
   *            Can be filled with the content of the response and a
   *            status code.
   * \param[in] buff
   *            A chunk of raw datas.
   * \param[out] request
   *            The HttpRequest to fill with the content of the request.
   *
   * \return A pointer to the consumed part of the buffer. If the
   *         parser find the "\r\n\r\n" sequence in the middle of the
   *         buffer it will return an iterator to the character after
   *         the last '\n', otherwise an iterator to the begin of
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
   * \return An empty Pipeline::ParsingRequestHandler when the
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa parsingHooks, ParsingRequestHandler
   */
  typedef Function<ParsingRequestHandler (Environment &)> ParsingHook;

  /**
   * \brief List of parsing hooks.
   *
   * Only one parser should return a valid request handler for a given
   * request, otherwise the parser with the highest priority will be
   * choosed.
   *
   * \sa ParsingHook, ParsingRequestHandler
   */
  std::list<std::pair<ParsingHook, float> > parsingHooks;

  /**
   * \brief Handler called after the parsing of a request.
   *
   * This hook point can be used for an URL rewrite module for
   * example.
   *
   * \param[out] response
   *            Can be filled with the content of the response and a
   *            status code.
   *
   * \sa postParsingHooks, PostParsingHook
   */
  typedef Function<void (HttpResponse & response)> PostParsingRequestHandler;

  /**
   * \brief Generate a Pipeline::PostParsingRequestHandler.
   *
   * \return An empty Pipeline::PostParsingRequestHandler when the
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa postParsingHooks, PostParsingRequestHandler
   */
  typedef Function<PostParsingRequestHandler (const Environment & environment,
                                              HttpRequest &       httpRequest)> PostParsingHook;

  /**
   * \brief List of post-parsing hooks.
   *
   * The server should called this hooks after the parsing of a
   * request.
   *
   * \sa PostParsingRequestHandler, PostParsingHook
   */
  std::list<std::pair<PostParsingHook, float> > postParsingHooks;

  /** @} */

  /**
   * \defgroup Bridge Bridge
   * \ingroup Pipeline
   *
   * \brief Contain hooks to run for the content generation.
   *
   * This is where modules like CGI, static pages, directory listing,
   * ... should be plugged.
   *
   * @{
   */

  /**
   * \brief Handler called to execute a request.
   *
   * This handler take an optional input from the body of the request
   * and generate a response body.
   *
   */
  class IContentRequestHandler
  {
  public:
    /**
     * \brief Callback for request body content
     *
     * \param [out] response
     *              Where the status code are filled.
     * \param [in] inBuffer
     *             A chunk of the body of the request.
     *
     * \retval true
     *    When the processing is finished.
     * \retval false
     *    When the processing is not finished.
     *
     * \sa contentHooks, ContentHook
     */
    virtual bool inContent(HttpResponse & response, const Buffer & inBuffer) = 0;

    /**
     * \brief Callback for response body content
     *
     * \param [out] response
     *              Where the status code are filled.
     * \param [in] inBuffer
     *             A chunk of the body of the request.
     *
     * \retval true
     *    When the processing is finished.
     * \retval false
     *    When the processing is not finished.
     *
     * \sa contentHooks, ContentHook
     */
    virtual bool outContent(HttpResponse & response, Buffer & outBuffer) = 0;

    /**
     * \brief Virtual destructor
     */
    virtual ~IContentRequestHandler() {};

    /**
     * \brief Dispose of the resources allocated by the request handler.
     *
     * \note This is often represented by a simple:
     *       \code delete this; \endcode
     */
    virtual void dispose() = 0;
  };

  /**
   * \brief Generate Pipeline::ContentRequestHandler.
   *
   * \param [out] fd If you set this param in your Hook, the server will
   *              register your fd in its event system (kqueue / epoll / select)
   *              and call back your handle on new activity.
   *
   * \warning Server has to set a default value (depending the system, for
   *          example -1 on UNIX integer, NULL on Windows HANDLE) for fd,
   *          to detect that hook provides a fd.
   *
   * \return An empty Pipeline::ContentRequestHandler when the
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa contentHooks, ContentRequestHandler
   */
  typedef Function<IContentRequestHandler *(const Environment & environment,
                                            const HttpRequest & request,
                                            FdType & fd)> ContentHook;


  /**
   * \brief List of content hooks.
   *
   * Only one content hooks can be used at for a given request, the
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
   * Appelé après la génération de contenu dynamique ou statique,
   * avec un buffer contenant des "morceaux" du contenu généré.
   *
   * \param[in] Buffer
   *          Les données générées à l'étape précédente de
   *          génération de contenu.
   * \param[out] Buffer
   *          Les données en sortie (peut être identique au buffer
   *          passé en entrée).
   */

  /**
   * \brief Hook to run after the content was generated by
   *        \c contentHooks.
   *
   * \param[out] reponse
   *            Where the status code are filled.
   * \param inBuffer
   *            A chunk of the body of the request.
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
   * \return An empty Pipeline::PostContentRequestHandler when the
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa postContentHooks, PostContentRequestHandler
   */
  typedef Function<PostContentRequestHandler (const Environment & environment,
                                              const HttpRequest & request)> PostContentHook;

  /**
   * \brief A list of post-content hooks.
   *
   * This hooks can be chained, but the highest priority will be
   * called first.
   *
   * \sa PostContentRequestHandler, PostContentHook
   */
  std::list<std::pair<PostContentHook, float> > postContentHooks;


  /**
   * \brief Handler to call after the postContentHooks were run.
   *
   * This hook can be used for a compression module for example.
   *
   * \param[out] response
   *            Where the status code are filled.
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
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa transformHooks, TransformRequestHandler
   */
  typedef Function<TransformRequestHandler (const Environment & environment,
                                            const HttpRequest & request)> TransformHook;

  /**
   * \brief List of transformation hooks.
   *
   * Transformation hooks are called handle by order of priority.
   *
   * \sa TransformRequestHandler, TransformHook
   */
  std::list<std::pair<TransformHook, float> > transformHooks;

  /**
   * \brief Handler called before sending data back to the client.
   *
   * \param[out] reponse
   *            Where the status code are filled.
   * \param inBuffer
   *            A chunk of the body of the request.
   * \param outBuffer
   *            An chunk of output body.
   *
   * \sa preSendHooks, PreSendHook
   */
  typedef Function<void (HttpResponse & response,
                         const Buffer &,
                         Buffer &)> PreSendRequestHandler;

  /**
   * \brief Generate Pipeline::PreSendRequestHandler.
   *
   * \return An empty Pipeline::PreSendRequestHandler when the
   *         hook has nothing to do for the request, otherwise a valid
   *         handler.
   *
   * \sa preSendHooks, PreSendRequestHandler
   */
  typedef Function<PreSendRequestHandler (const Environment &, const HttpRequest &)> PreSendHook;

  /**
   * \brief A list of pre-send hooks.
   *
   * This hooks can be chained, but the highest priority will be
   * called first.
   *
   * \sa PreSendRequestHandler, PreSendHook
   */
std::list<std::pair<PreSendHook, float> > preSendHooks;

  /** @} */

};

/** @} */

} // ! bref

#endif /* !BREF_API_PIPELINE_H */
