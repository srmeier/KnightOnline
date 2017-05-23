var globalUrl = "/Content/";

function openCustomRoxy() {
    $('.DosyaYoneticisi>iframe').attr('src',  globalUrl + 'tinymce/plugins/fileman/index.html?integration=custom&type=image');

    $('.DosyaYoneticisi').dialog({ modal: true, width: 875, height: 600 });
}
function closeCustomRoxy() {
    $('.DosyaYoneticisi').dialog('close');
}
$('.fileman').click(function () {
    openCustomRoxy();
});
function tik(clasID) {
    $('.' + clasID).remove();
    return false;
}

tinymce.init({
    mode: "specific_textareas",
    editor_selector : /(mceEditor|mceRichText)/,
    theme: "modern",
    menubar: false,
    plugins: [
         "autolink link image lists  preview hr",
         "searchreplace wordcount visualblocks  code media ",
         "table contextmenu directionality emoticons paste textcolor dosya textpattern"
    ],
    content_css: globalUrl+"tinymce/skins/lightgray/content.min.css",
    toolbar: "bold italic  underline  | forecolor backcolor emoticons hr  | alignleft aligncenter alignright alignjustify | bullist numlist outdent indent | link image media | table preview code  ", file_browser_callback: RoxyFileBrowser
});

function RoxyFileBrowser(field_name, url, type, win) {
    var roxyFileman = globalUrl+'TinyMce/plugins/fileman/index.html?integration=tinymce4';
    if (roxyFileman.indexOf("?") < 0) {
        roxyFileman += "?type=" + type;
    }
    else {
        roxyFileman += "&type=" + type;
    }
    roxyFileman += '&input=' + field_name + '&value=' + document.getElementById(field_name).value;
    tinyMCE.activeEditor.windowManager.open({
        file: roxyFileman,
        title: 'Dosya Yöneticisi',
        width: 850,
        height: 650,
        resizable: "yes",
        plugins: "media",
        inline: "yes",
        close_previous: "no"
    }, { window: win, input: field_name });
    return false;
}