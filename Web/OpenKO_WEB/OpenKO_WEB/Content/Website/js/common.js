<!--
function $() {
    var elements = new Array();
    for (var i = 0; i < arguments.length; i++) {
	    var element = arguments[i];
	    if (typeof element == 'string')
		    element = document.getElementById(element);
	    if (arguments.length == 1)
		    return element;
	    elements.push(element);
    }
    return elements;
}

function getElementsByClass(searchClass,node,tag) {
	var elements = new Array();
	if ( node == null )
		node = document;
	if ( tag == null )
		tag = '*';
	var els = node.getElementsByTagName(tag);
	//var pattern = new RegExp('(^|\\\\s)'+searchClass+'(\\\\s|$)');
	//var pattern = new RegExp("(^|\\s)"+searchClass+"(\\s|$)");
	for (var i = 0; i < els.length; i++) {
		//if ( pattern.test(els[i].className) ) {
		if (els[i].className == searchClass)
			elements.push(els[i]);
	}
	//if (elements.length == 0) return false;
	//if (elements.length == 1) return elements[0];
	return elements; 
}

function toggle() {
  for (var i=0, el; el = $(arguments[i]); i++) {
    el.style.display = (el.style.display != 'none' ? 'none' : '' );
  }
}

function addEvent( obj, type, fn ) {
	if (obj.addEventListener) {
		obj.addEventListener( type, fn, false );
		EventCache.add(obj, type, fn);
	}
	else if (obj.attachEvent) {
		obj["e"+type+fn] = fn;
		obj[type+fn] = function() { obj["e"+type+fn]( window.event ); }
		obj.attachEvent( "on"+type, obj[type+fn] );
		EventCache.add(obj, type, fn);
	}
	else {
		obj["on"+type] = obj["e"+type+fn];
	}
}
	
var EventCache = function(){
	var listEvents = [];
	return {
		listEvents : listEvents,
		add : function(node, sEventName, fHandler){
			listEvents.push(arguments);
		},
		flush : function(){
			var i, item;
			for(i = listEvents.length - 1; i >= 0; i = i - 1){
				item = listEvents[i];
				if(item[0].removeEventListener){
					item[0].removeEventListener(item[1], item[2], item[3]);
				};
				if(item[1].substring(0, 2) != "on"){
					item[1] = "on" + item[1];
				};
				if(item[0].detachEvent){
					item[0].detachEvent(item[1], item[2]);
				};
				item[0][item[1]] = null;
			};
		}
	};
}();
addEvent(window,'unload',EventCache.flush);
//-->