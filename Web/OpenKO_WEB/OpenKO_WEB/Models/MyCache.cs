using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace OpenKO_WEB.Models
{
    public class MyCache
    {
        public static List<tblSecretQuestion> cQuestionList { get; set; }
        public static List<tblCountry> cCountryList { get; set; }
        public static void Load()
        {
            Account_DBEntities AccountDB = new Account_DBEntities();
            OpenKO_WebEntities WebDB = new OpenKO_WebEntities();
			cQuestionList = WebDB.tblSecretQuestion.ToList();
			cCountryList = WebDB.tblCountry.ToList();
		}
	}
}