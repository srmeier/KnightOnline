using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using OpenKO_WEB.Models;
namespace OpenKO_WEB.Controllers
{
    public class HomeController : Controller
    {
        #region Database Connections
        public OpenKO_WebEntities WebDB = new OpenKO_WebEntities();
        public Account_DBEntities AccountDB = new Account_DBEntities();
        #endregion
        #region Home Page
        public ActionResult Index()
        {
            return View();
        }

        public ActionResult _News(short Type, short Count = 0)
        {
            //Types ; 1 News, 2 Announcements, 3 Updates
            ViewBag.Type = Type;
            if (Count > 0)
                return View(WebDB.tblNews.OrderByDescending(d => d.cDate).Where(d => d.TypeId == Type && d.Status).Take(Count).ToList());
            else
                return View(WebDB.tblNews.OrderByDescending(d => d.cDate).Where(d => d.TypeId == Type && d.Status).ToList());
        }
        #endregion
        #region Login & Logout
        public ActionResult Login()
        {
            return View();
        }

        [HttpPost]
        public ActionResult Login(string Username, string Password)
        {
            if (string.IsNullOrEmpty(Username))
            {
                TempData["Hata"] = "AccountID can not be null";
                return View();
            }
            if (string.IsNullOrEmpty(Password))
            {
                TempData["Hata"] = "Password can not be null";
                return View();
            }
            TB_USER User = AccountDB.TB_USER.FirstOrDefault(d => d.strAccountID == Username && d.strPasswd == Password);
            if (User != null)
            {
                Functions.LoadUser(User);
                return RedirectToAction("Index", "Home");
            }
            else
            {
                TempData["Hata"] = "AccountID or password wrong!";
                return RedirectToAction("Index", "Home");
            }
        }

        public ActionResult Logout()
        {
            Functions.Logout();
            return Redirect(Url.Content("~/Home/Index"));
        }
        #endregion
        #region Register
        public ActionResult Register()
        {
            return View();
        }

        [HttpPost]
        public ActionResult Register(TB_USER User, string repassword, string agree)
        {
            #region Required fills
            if (User.strAccountID == null)
            {
                TempData["Error"] = "Error : AccountID can not be null.";
                return View(User);
            }
            if (User.strPasswd == null)
            {
                TempData["Error"] = "Error : Password can not be null.";
                return View(User);
            }
            if (User.strPasswd!=repassword)
            {
                TempData["Error"] = "Error : Passwords does not match.";
                return View(User);
            }
            if (repassword == null)
            {
                TempData["Error"] = "Error : Password confirm can not be null.";
                return View(User);
            }

            if (User.strEmail == null)
            {
                TempData["Error"] = "Error : Email address can not be null.";
                return View(User);
            }

            if (User.sQuestionId == 0)
            {
                TempData["Error"] = "Error : Please select Secret Question.";
                return View(User);
            }
            if (User.sQuestionAnswer == null)
            {
                TempData["Error"] = "Error : Please enter secret question answer.";
                return View(User);
            }
            if (agree != "Y")
            {
                TempData["Error"] = "Error : You must read and agree our Terms of Service.";
                return View(User);
            }
            if (!Functions.ValidAccountID(User.strAccountID))
            {
                TempData["Error"] = "Please enter a valid Account ID.";
                return View(User);
            }
            if (!Functions.ValidAccountID(User.strPasswd))
            {
                TempData["Error"] = "Please enter a valid Password.";
                return View(User);
            }
            if (!Functions.ValidEmail(User.strEmail))
            {
                TempData["Error"] = "Please enter a valid email address.";
                return View(User);
            }
            #endregion
            TB_USER accid = AccountDB.TB_USER.FirstOrDefault(d => d.strAccountID == User.strAccountID);
            if (accid != null)
            {
                TempData["Error"] = "Error : This Account ID already in use.";
                return View(User);
            }
            TB_USER email = AccountDB.TB_USER.FirstOrDefault(d => d.strEmail == User.strEmail);
            if (email != null)
            {
                TempData["Error"] = "Error : This Email already in use.";
                return View(User);
            }
       
            TB_USER dbUser = new TB_USER();
            dbUser.strAccountID = User.strAccountID;
            dbUser.strPasswd = User.strPasswd;
            dbUser.strSocNo = "1";
            dbUser.strEmail = User.strEmail;
            dbUser.strAuthority = 1;
            dbUser.sQuestionId = User.sQuestionId;
            dbUser.sQuestionAnswer = User.sQuestionAnswer;
            dbUser.PremiumExpire = DateTime.Now.AddDays(3);
            dbUser.CountryId = User.CountryId;
            AccountDB.TB_USER.Add(dbUser);
            try
            {
                TempData["Information"] = "Your account has been created!";
                AccountDB.SaveChanges();
            }
            catch (Exception)
            {
                throw;
            }
            return View();
        }
        #endregion
        #region Pages
        public ActionResult Features()
        {
            return View();
        }

        public ActionResult Story()
        {
            return View();
        }

        public ActionResult GameClient()
        {
            return View();
        }

        public ActionResult Wallpaper()
        {
            return View();
        }

        public ActionResult Screenshots()
        {
            return View();
        }

        public ActionResult ConceptArt()
        {
            return View();
        }

        public ActionResult Video()
        {
            return View();
        }

        public ActionResult FAQ()
        {
            return View();
        }
        public ActionResult FAQGamePlay()
        {
            return View();
        }
        public ActionResult FAQAccount()
        {
            return View();
        }
        public ActionResult CustomerSupport()
        {
            return View();
        }

        public ActionResult Policies()
        {
            return View();
        }

        public ActionResult LunarWar()
        {
            return View();
        }

        public ActionResult DarkLunarWar()
        {
            return View();
        }

        public ActionResult CastleSiegeWar()
        {
            return View();
        }

        public ActionResult ForgettenTemple()
        {
            return View();
        }
        public ActionResult NewPlayers()
        {
            return View();
        }

        public ActionResult AdvancedPlay()
        {
            return View();
        }

        public ActionResult Crafting()
        {
            return View();
        }
        public ActionResult Bosses()
        {
            return View();
        }

        public ActionResult Glossary()
        {
            return View();
        }
        #endregion
    }
}