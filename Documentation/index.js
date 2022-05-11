let $plugins=new Map();let $pending_plugins=new Map();let $get_plugin=function(name){if($plugins.has(name)){return $plugins.get(name);}else{let $plugin=$pending_plugins.get(name)();$plugins.set(name,$plugin);$pending_plugins.delete(name);return $plugin;};};
require("./neutrino_plugins/files.js")($pending_plugins,$get_plugin);
require("./neutrino_plugins/stats.js")($pending_plugins,$get_plugin);
require("./neutrino_plugins/DOM.js")($pending_plugins,$get_plugin);
for(var[$key,$value]of $pending_plugins.entries()){$get_plugin($key);};
let files=$plugins.get('files'); //2:7
let DOM=$plugins.get('DOM'); //3:7
files.deleteDirectory("./Documentation"); //6:22 | index.nt
files.createDirectory("./Documentation"); //7:22 | index.nt
let nodehp=require("./nodehp.js"); //10:13 | index.nt
let pages=files.getFiles("./pages",0); //12:12 | index.nt
for(var[i,file]of pages.entries()){
	if(file.type!="directory"){
		let output=nodehp("./pages/"+(file.name)+".nodehp",{
			DOM:DOM,
			message:"hi"
		}); //16:21 | index.nt
		files.writeFile("./Documentation/"+(file.name)+".html",output); //24:24 | index.nt
	}; //15:7 | index.nt
}; //14:8 | index.nt
