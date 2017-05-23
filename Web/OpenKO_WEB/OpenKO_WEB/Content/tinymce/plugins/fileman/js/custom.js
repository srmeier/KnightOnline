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
var tikDegisken = 0;
function FileSelected(file){

    var sss = file.ext + file.size + file.time + tikDegisken;
    $(window.parent.document).find('.Image').append('<li class="' + sss + '"><input type="hidden" name="Resimler" value="' + file.fullPath + '" /><img  src="' + file.path + '/thumbnail/' + file.name + '" alt="" /><div class="modify_image"><span onclick=" return tik(\'' + sss + '\');"  class="iconsweet tip_north ResimSil" title="Kaldýr">X</span></div></li>');
    
    window.parent.closeCustomRoxy();
    //var link = '<a href="/Content/tinymce/main.ashx?a=DOWNLOAD&f=' + file.fullPath + '" title="' + file.name + '"  target="_blank"  >Dosyayi Indir : ' + file.name + ' </a>';
    //      var win = (window.opener ? window.opener : window.parent);
    //      win.document.getElementById(win.tinymce.activeEditor.id + '_ifr').contentDocument.getElementById('tinymce').innerHTML += link;
    //      win.tinyMCE.activeEditor.windowManager.close();

    // onsubmit(file);
    tikDegisken++;
}

// this function is called when folder is expanded and only if screen width is smaller than 1024px
function ResizeForTouchDevices(width, height){
  // try to set the height for the window or frame in which fileman is opened
  try{
    if(window.frameElement){
      window.frameElement.style.height = height + 'px';
      window.frameElement.height = height;
    }
    else
      window.resizeTo(width, height);
  }
  catch(ex){}
  
  // try to set the height of JQuery dialog
  try{
    var d = $(window.frameElement).closest('.ui-widget-content');
    d.css('height', height);
  }
  catch(ex){}
  
  // try to set the height if is opened by TinyMCE
  try{
    $(window.frameElement).closest('.mce-container-body').css('height', height);
  }
  catch(ex){}
}