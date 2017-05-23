using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO.Compression;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace OpenKO_WEB.Models
{
    public class LoginAttribute : ActionFilterAttribute, IActionFilter
    {
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            if (General.User == null)
            {
                string Redirect = filterContext.HttpContext.Request.Url.PathAndQuery;
                string RedirectUrl = "";
                if (!string.IsNullOrEmpty(Redirect))
                {
                    NameValueCollection queryString = System.Web.HttpUtility.ParseQueryString(string.Empty);
                    queryString["Redirect"] = Redirect;
                    RedirectUrl = "?" + queryString.ToString();
                }
                if (filterContext.HttpContext.Request.IsAjaxRequest())
                    filterContext.Result = new RedirectResult("/Home/Index");
                else
                    filterContext.Result = new RedirectResult("/Home/Index" + RedirectUrl);
            }
        }
    }
}