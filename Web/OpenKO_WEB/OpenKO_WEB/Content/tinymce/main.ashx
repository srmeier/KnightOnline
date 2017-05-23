<%@ WebHandler Language="C#" Class="RoxyFilemanHandler" Debug="true" %>
/*
  RoxyFileman - web based file manager. Ready to use with CKEditor, TinyMCE. 
  Can be easily integrated with any other WYSIWYG editor or CMS.

  Copyright (C) 2013, RoxyFileman.com - Lyubomir Arsov. All rights reserved.
  For licensing, see LICENSE.txt or http://RoxyFileman.com/license

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Contact: Lyubomir Arsov, liubo (at) web-lobby.com
*/
using System;
using System.Reflection;
using System.Drawing;
using System.Collections;
using System.IO;
using System.Collections.Generic;
using System.Web;
using System.Text.RegularExpressions;
using System.Drawing.Imaging;
using System.IO.Compression;

public class RoxyFilemanHandler : IHttpHandler, System.Web.SessionState.IRequiresSessionState
{
    Dictionary<string, string> _settings = null;
    Dictionary<string, string> _lang = null;
    HttpResponse _r = null;
    HttpContext _context = null;
    string confFile = "/conf.json";
    public void ProcessRequest (HttpContext context) {
        _context = context;
        _r = context.Response;
        string action = "DOWNLOAD";
        try{
            if (_context.Request["a"] != null)
                action = (string)_context.Request["a"];
            
            VerifyAction(action);
            switch (action.ToUpper())
            {
                case "DOWNLOAD":
                    DownloadFile("/Content/Upload/Dosya"+_context.Request["f"]);
                    break;
                case "REDIRECT":
                    RedirectFile("/Content/Upload/Dosya"+_context.Request["f"]);                    
                    break;
                default:
                    _r.Write(GetErrorRes("Boyle Bir Method Yoktur."));
                    break;
            }
        
        }
        catch(Exception ex){
            if (action == "UPLOAD"){
                _r.Write("<script>");
                _r.Write("parent.fileUploaded(" + GetErrorRes(LangRes("E_UploadNoFiles")) + ");");
                _r.Write("</script>");
            }
            else{
            _r.Write(GetErrorRes(ex.Message));
            }
        }
        
    }
    private string FixPath(string path)
    {
        if (!path.StartsWith("~")){
            if (!path.StartsWith("/"))
                path = "/" + path;
            path = "~" + path;
        }
        return _context.Server.MapPath(path);
    }
    private string GetLangFile(){
      return "../lang/tr.json";
    }
    protected string LangRes(string name)
    {
        string ret = name;
        if (_lang == null)
            _lang = ParseJSON(GetLangFile());
        if (_lang.ContainsKey(name))
            ret = _lang[name];

        return ret;
    }
    protected string GetFileType(string ext){
        string ret = "file";
        ext = ext.ToLower();
        if(ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".gif")
            ret = "image";
          else if(ext == ".swf" || ext == ".flv")
            ret = "flash";
        return ret;
    }
    protected bool CanHandleFile(string filename)
    {
        bool ret = false;
        FileInfo file = new FileInfo(filename);
        string ext = file.Extension.Replace(".", "").ToLower();
        string setting = GetSetting("FORBIDDEN_UPLOADS").Trim().ToLower();
        if (setting != "")
        {
            ArrayList tmp = new ArrayList();
            tmp.AddRange(Regex.Split(setting, "\\s+"));
            if (!tmp.Contains(ext))
                ret = true;
        }
        setting = GetSetting("ALLOWED_UPLOADS").Trim().ToLower();
        if (setting != "")
        {
            ArrayList tmp = new ArrayList();
            tmp.AddRange(Regex.Split(setting, "\\s+"));
            if (!tmp.Contains(ext))
                ret = false;
        }
        
        return ret;
    }
    protected Dictionary<string, string> ParseJSON(string file){
        Dictionary<string, string> ret = new Dictionary<string,string>();
        string json = "";
        try{
            json = File.ReadAllText(_context.Server.MapPath(file), System.Text.Encoding.UTF8);
        }
        catch(Exception ex){}

        json = json.Trim();
        if(json != ""){
            if (json.StartsWith("{"))
                json = json.Substring(1, json.Length - 2);
            json = json.Trim();
            json = json.Substring(1, json.Length - 2);
            string[] lines = Regex.Split(json, "\"\\s*,\\s*\"");
            foreach(string line in lines){
                string[] tmp = Regex.Split(line, "\"\\s*:\\s*\"");
                try{
                    if (tmp[0] != "" && !ret.ContainsKey(tmp[0]))
                    {
                       ret.Add(tmp[0], tmp[1]);
                    }
                }
                catch(Exception ex){}
            }
        }
        return ret;
    }
    protected string GetFilesRoot(){
        string ret = GetSetting("FILES_ROOT");
        if (GetSetting("SESSION_PATH_KEY") != "" && _context.Session[GetSetting("SESSION_PATH_KEY")] != null)
            ret = (string)_context.Session[GetSetting("SESSION_PATH_KEY")];
        
        if(ret == "")
            ret = _context.Server.MapPath("~/Content/Upload/Dosya");
        else
            ret = FixPath(ret);
        return ret;
    }
    protected void LoadConf(){
        if(_settings == null)
            _settings = ParseJSON(confFile);
    }
    protected string GetSetting(string name){
        string ret = "";
        LoadConf();
        if(_settings.ContainsKey(name))
            ret = _settings[name];
        
        return ret;
    }
    protected void CheckPath(string path)
    {
        if (FixPath(path).IndexOf(GetFilesRoot()) != 0)
        {
            throw new Exception("Access to " + path + " is denied");
        }
    }
    protected void VerifyAction(string action)
    {
        string setting = GetSetting(action);
        if (setting.IndexOf("?") > -1)
            setting = setting.Substring(0, setting.IndexOf("?"));
        if (!setting.StartsWith("/"))
            setting = "/" + setting;
        setting = ".." + setting;
        
        //if (_context.Server.MapPath(setting) != _context.Server.MapPath(_context.Request.Url.LocalPath))
        //    throw new Exception(LangRes("E_ActionDisabled"));
    }
    protected string GetResultStr(string type, string msg)
    {
        return "{\"res\":\"" + type + "\",\"msg\":\"" + msg.Replace("\"","\\\"") + "\"}";
    }
    protected string GetSuccessRes(string msg)
    {
        return GetResultStr("ok", msg);
    }
    protected string GetSuccessRes()
    {
        return GetSuccessRes("");
    }
    protected string GetErrorRes(string msg)
    {
        return GetResultStr("error", msg);
    }

    protected void DownloadFile(string path)
    {
        CheckPath(path);
        FileInfo file = new FileInfo(FixPath(path));
        if(file.Exists){
            _r.Clear();
            _r.Headers.Add("Content-Disposition", "attachment; filename=\"" + file.Name + "\"");
            _r.ContentType = "application/force-download";
            _r.TransmitFile(file.FullName);
            _r.Flush();
            _r.End();
        }
    }
    protected void RedirectFile(string path) {
        CheckPath(path);
        FileInfo file = new FileInfo(FixPath(path));
        if (file.Exists)
        {
            _r.Redirect("/Home/DosyaFormu?Path="+path);
        }
    }
  
 
    public bool IsReusable {
        get {
            return false;
        }
    }

}