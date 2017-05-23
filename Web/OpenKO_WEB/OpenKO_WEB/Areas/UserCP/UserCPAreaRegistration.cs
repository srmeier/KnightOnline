using System.Web.Mvc;

namespace OpenKO_WEB.Areas.UserCP
{
    public class UserCPAreaRegistration : AreaRegistration
    {
        public override string AreaName
        {
            get
            {
                return "UserCP";
            }
        }

        public override void RegisterArea(AreaRegistrationContext context)
        {
            context.MapRoute(
              "UserCP_default",
              "UserCP/{controller}/{action}/{id}",
              new { action = "Index", id = UrlParameter.Optional }
          );
        }
    }
}