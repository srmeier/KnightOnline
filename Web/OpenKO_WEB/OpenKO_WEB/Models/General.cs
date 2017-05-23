using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using OpenKO_WEB.Models;
using System.Web.Mvc;
using System.Web.SessionState;

namespace OpenKO_WEB
{
    public class General
    {
        public static TB_USER User
        {
            get { return Functions.BringUser(); }
            set { Functions.LoadUser(value); }
        }
        public static HttpRequest Request
        {
            get { return HttpContext.Current.Request; }
        }
        public static HttpSessionState Session
        {
            get { return HttpContext.Current.Session; }
        }
        public static HttpApplicationState Application
        {
            get { return HttpContext.Current.Application; }
        }
        public static HttpResponse Response
        {
            get { return HttpContext.Current.Response; }
        }
        public static bool IsAjaxRequest
        {
            get { return new HttpRequestWrapper(System.Web.HttpContext.Current.Request).IsAjaxRequest(); }
        }
        public static HttpServerUtility Server
        {
            get { return HttpContext.Current.Server; }
        }
        public static System.Uri Url
        {
            get { return HttpContext.Current.Request.Url; }
        }
        public static string RootUrl
        {
            get { return HttpRuntime.AppDomainAppVirtualPath; }
        }
        public static string Domain
        {
            get { return "Http://" + General.Url.Authority; }
        }
        public static string controllerName
        {
            get { return HttpContext.Current.Request.RequestContext.RouteData.DataTokens["controller"].ToString(); }
        }
        public static string ActionName
        {
            get { return HttpContext.Current.Request.RequestContext.RouteData.DataTokens["Action"].ToString(); }
        }
    }
}