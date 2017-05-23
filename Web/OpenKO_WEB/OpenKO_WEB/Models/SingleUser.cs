using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace OpenKO_WEB
{
    public class CacheUser
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public DateTime LoginDate { get; set; }
        public string LoginIP { get; set; }
        public string LoginServer { get; set; }
        public string SessionId { get; set; }
    }
    public class SingleUser
    {
        static List<CacheUser> _UserList;
        public static List<CacheUser> UserList
        {
            get { return SingleUser._UserList ?? new List<CacheUser>(); }
        }
        static string CookieSessionId
        {
            get
            {
                var CookieSession = General.Request.Cookies[Static.Cookie.CookieSessionId];
                if (CookieSession == null)
                    CookieSession = new HttpCookie(Static.Cookie.CookieSessionId, General.Session.SessionID);
                if (CookieSession.Expires < DateTime.Now)
                {
                    CookieSession.Value = General.Session.SessionID;
                    CookieSession.Expires = DateTime.Now.AddDays(1);
                    General.Response.Cookies.Add(CookieSession);
                }
                return CookieSession.Value;
            }
        }
        public static void AddUser(string Adi, int Id)
        {
            CacheUser IncomingUser = new CacheUser
            {
                LoginIP = General.Request.UserHostAddress,
                LoginServer = General.Request.UserHostName,
                Name = Adi,
                Id = Id,
                LoginDate = DateTime.Now,
                SessionId = CookieSessionId
            };
            var MyApplication = General.Application;
            MyApplication.Lock();
            _UserList = ((List<CacheUser>)MyApplication[Static.Cache.SingleUser]) ?? new List<CacheUser>();
            var UsersInSystem = _UserList.Where(d => d.Id == IncomingUser.Id);
            if (UsersInSystem != null)
                foreach (var SistemdekiKullanici in UsersInSystem.ToList())
                {
                    _UserList.Remove(SistemdekiKullanici);
                }

            _UserList.Add(IncomingUser);

            MyApplication[Static.Cache.SingleUser] = _UserList;
            MyApplication.UnLock();
        }
        public static CacheUser BringUser(int Id)
        {
            return _UserList.FirstOrDefault(d => d.Id == Id);
        }
        public static bool CookieIdExist(int UserID)
        {
            return UserList.Count(d => d.Id == UserID) == 0 || UserList.Any(d => d.Id == UserID && d.SessionId == CookieSessionId);
        }

        public static bool UserExist(int UserID)
        {
            return UserList.Any(d => d.Id == UserID);
        }
        public static void DeleteUser(int UserID)
        {
            var MyApplication = General.Application;
            MyApplication.Lock();
            _UserList = ((List<CacheUser>)MyApplication[Static.Cache.SingleUser]) ?? new List<CacheUser>();
            var UsersInSystem = _UserList.Where(d => d.Id == UserID);
            if (UsersInSystem != null)
                foreach (var UserInSystem in UsersInSystem.ToList())
                {
                    _UserList.Remove(UserInSystem);
                }
            MyApplication[Static.Cache.SingleUser] = _UserList;
            MyApplication.UnLock();
            HttpCookie ck = General.Request.Cookies[Static.Cookie.CookieSessionId];
            if (ck != null)
            {
                Random Rdm = new Random();
                ck.Expires = DateTime.Now.AddDays(1);
                ck.Value = Rdm.Next().ToString();
                General.Response.Cookies.Add(ck);
            }
        }
    }
}