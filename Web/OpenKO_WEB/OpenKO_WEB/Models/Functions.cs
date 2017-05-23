using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using OpenKO_WEB.Models;
using System.Text.RegularExpressions;

namespace OpenKO_WEB
{
    public class Functions
    {
        #region Basic requirements
        public static string ResolveServerUrl(string serverUrl, bool forceHttps)
        {
            if (serverUrl.IndexOf("://") > -1)
                return serverUrl;
            string newUrl = serverUrl;
            Uri originalUri = System.Web.HttpContext.Current.Request.Url;
            newUrl = (forceHttps ? "https" : originalUri.Scheme) +
                "://" + originalUri.Authority + newUrl;
            return newUrl;
        }
        public static bool ValidEmail(string Email)
        {
            bool Valid = true;
            string emailRegex = @"^([a-zA-Z0-9_\-\.]+)@((\[[0-9]{1,3}" +
                                             @"\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([a-zA-Z0-9\-]+\" +
                                                @".)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$";
            Regex re = new Regex(emailRegex);
            if (!re.IsMatch(Email))
                Valid = false;
            return Valid;
        }
        public static bool ValidAccountID(string AccountID)
        {
            bool Valid = true;
            string accRegex = @"^(?=[A-Za-z0-9])(?!.*[._()\[\]-]{2})[A-Za-z0-9._()\[\]-]{3,15}$";
            Regex re = new Regex(accRegex);
            if (!re.IsMatch(AccountID))
                Valid = false;
            return Valid;
        }
        #endregion
        #region Login & Load & Bring & Logout
        public static void LoadUser(TB_USER User)
        {
            General.Session[Static.Session.User] = User;
            HttpCookie ck = General.Request.Cookies[Static.Cookie.User];
            if (ck == null)
                ck = new HttpCookie(Static.Cookie.User, Crypto.EncryptStringAES(User.Id.ToString(), Static.Cookie.SharedSecret));
            if (ck.Expires < DateTime.Now)
                ck.Value = Crypto.EncryptStringAES(User.Id.ToString(), Static.Cookie.SharedSecret);
            ck.Expires = DateTime.Now.AddDays(1);
            General.Response.Cookies.Add(ck);
            SingleUser.AddUser(User.strAccountID, User.Id);
        }
        public static TB_USER BringUser()
        {
            Account_DBEntities AccountDB = new Account_DBEntities();
            if (General.Session[Static.Session.User] == null)
            {
                if (new HttpRequestWrapper(General.Request).IsAjaxRequest())
                {
                    return null;
                }
                HttpCookie ck = General.Request.Cookies[Static.Session.User];
                if (ck == null)
                    return null;
                else
                {
                    int ID = 0;
                    int.TryParse(Crypto.DecryptStringAES(ck.Value, Static.Cookie.SharedSecret), out ID);
                    if (ID <= 0)
                        return null;

                    if (!SingleUser.CookieIdExist(ID))
                    {
                        return null;
                    }
                    TB_USER User = AccountDB.TB_USER.FirstOrDefault(d => d.Id == ID);
                    if (User != null)
                    {
                        LoadUser(User);
                    }
                    else
                        return null;
                    return User;
                }
            }
            else
            {
                TB_USER User = General.Session[Static.Session.User] as TB_USER;
                if (!SingleUser.CookieIdExist(User.Id))
                {
                    return null;
                }
                return User;
            }
        }


        public static bool Logout()
        {
            try
            {
                string Eposta = General.User.strEmail;
                HttpContext.Current.Session.Abandon();
                HttpContext.Current.Session.Clear();
                HttpContext.Current.Response.Cookies.Clear();

                var ck = General.Response.Cookies[Static.Cookie.User];
                if (ck != null)
                    ck.Expires = DateTime.Now.AddDays(-1);
            }
            catch (Exception)
            {
                return false;
            }
            return true;
        }
        #endregion
        #region Get Cache Lists
        public static List<SelectListItem> GetSecretQuestionList(int SelectedId = 0)
        {
            List<SelectListItem> SelectList = MyCache.cQuestionList.OrderBy(d => d.Question).Select(d => new SelectListItem { Text = d.Question, Value = d.Id.ToString(), Selected = (d.Id == SelectedId) ? true : false }).ToList();
            SelectList.Insert(0, new SelectListItem { Text = "Select", Value = null });
            return SelectList;
        }
        public static List<SelectListItem> GetCountryList(int SelectedId = 0)
        {
            List<SelectListItem> SelectList = MyCache.cCountryList.OrderBy(d => d.Country).Select(d => new SelectListItem { Text = d.Country, Value = d.Id.ToString(), Selected = (d.Id == SelectedId) ? true : false }).ToList();
            SelectList.Insert(0, new SelectListItem { Text = "Select", Value = null });
            return SelectList;
        }

        public static string GetCountry(int Id)
        {
            return MyCache.cCountryList.FirstOrDefault(d => d.Id == Id).Country;
        }

        public static string GetQuestion(int Id)
        {
            return MyCache.cQuestionList.FirstOrDefault(d => d.Id == Id).Question;
        }
        #endregion
    }
}