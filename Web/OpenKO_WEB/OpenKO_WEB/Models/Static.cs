using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace OpenKO_WEB
{
    public class Static
    {
        public class Session
        {
            public const string User = "User";
        }
        public class Cookie
        {
            public const string User = "User";
            public const string CookieSessionId = "xcode";
            public const string SharedSecret = "cookie_";
        }
        public class Cache
        {
            public const string SingleUser = "SingleUser";
        }
    }
}