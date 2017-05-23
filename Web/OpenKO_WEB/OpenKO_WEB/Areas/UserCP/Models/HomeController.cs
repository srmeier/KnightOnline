using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using OpenKO_WEB.Models;
namespace OpenKO_WEB.Areas.UserCP.Models
{
    [Login]
    public class HomeController : Controller
    {
        public OpenKO_WebEntities WebDB = new OpenKO_WebEntities();
        public Account_DBEntities AccountDB = new Account_DBEntities();
        public ActionResult Index()
        {
            return View();
        }

        public ActionResult ChangePassword()
        {
            return View();
        }
        [HttpPost]
        public ActionResult ChangePassword(string sQuestionAnswer, string NewPassword, string NewPasswordAgain)
        {
            if (sQuestionAnswer == null)
            {
                TempData["Error"] = "Error : Please enter secret answer.";
                return View("ChangePassword");
            }
            if (NewPassword != NewPasswordAgain)
            {
                TempData["Error"] = "Error : Passwords does not match.";
                return View("ChangePassword");
            }
            TB_USER dbUser = AccountDB.TB_USER.FirstOrDefault(d => d.strAccountID == General.User.strAccountID);
            if (dbUser.sQuestionAnswer != sQuestionAnswer)
            {
                TempData["Error"] = "Error : Answer is wrong.";
                return View("ChangePassword");
            }
            dbUser.strPasswd = NewPassword;
            try
            {
                AccountDB.SaveChanges();
                TempData["Information"] = "Password has been updated successfully!";
            }
            catch (Exception)
            {
                TempData["Error"] = "Error : An error occurred";
            }
            return View("ChangePassword");
        }
    }
}