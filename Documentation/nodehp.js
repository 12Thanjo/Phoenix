// THIS IS A MODIFIED VERSION OF NODEHP
// THIS IS NOT THE ORIGINAL



var monokai = {
	background: "#23241f",
	red:    "#f92472",
	orange: "#fd9622",
	yellow: "#e7db74",
	green:  "#a6e22b",
	cyan: 	"#67d8ef",
	purple: "#ac80ff",
	gray:   "#74705d",
}


var regex = {
	digit: "[0-9]",
	integer: "[-]?[0-9]+",
	number: "[-]?([0-9]*[.])?[0-9]+",
	letter: "[a-zA-Z]",
	word: "[a-zA-Z]+",
	endl: ".*",
	whitespace: "( |\t|\n|\r)",
	singleQuotedString: "'(\\\\.|[^'\\\\])*'",
	doubleQuotedString: '\\"(\\\\.|[^"\\\\])*\\"',
	capture: function(start, end){
		return start + "(\\\\.|[^" + end + "\\\\])*" + end;
	},
};

var tokens = [];
var Token = function(regex, action){
	this.regex = new RegExp("^" + regex);
	this.action = action;
	tokens.push(this);
}



//////////////////////////////////////////////////////////////////////////




// check string for keyword
var _CSFKW_ = function(string, cursor, keyword){
	for(var i=0; i<keyword.length;i++){
		if(string[cursor+i] != keyword[i]){
			return false;
		}
	}
	return true;
}

var _NODEHP_ = (_PATH_, data)=>{
	var _FILE_ = require('fs').readFileSync(_PATH_, 'utf8');

	var _OUTPUT_ = "";

	var indentation = 0;

	for(var _CURSOR_=0; _CURSOR_<_FILE_.length;_CURSOR_++){
		if(_CSFKW_(_FILE_, _CURSOR_, '<nodehp>')){
			_CURSOR_ += 8;
			var _STRING_ = "";
			while(!_CSFKW_(_FILE_, _CURSOR_, '</nodehp>')){
				_STRING_ += _FILE_[_CURSOR_];
				_CURSOR_ += 1;
			}
			_CURSOR_ += 9;
			var echo = function(_STR_){
				_OUTPUT_ += _STR_;
				return _STR_;
			}

			var include = function(_IPATH_, _IDATA_){
				_OUTPUT_ += _NODEHP_(_IPATH_, _IDATA_);
			}

			var tag = function(_TNAME_, _STR_){
				if(_STR_ == null){_STR_ = "";}
				return `<${_TNAME_}>${_STR_}</${_TNAME_}>`;
			}			

			var tag_style = function(_TNAME_, _INL_, _STR_){
				if(_INL_ == null){_INL_ = "";}else{_INL_ = " " + _INL_;}
				if(_STR_ == null){_STR_ = "";}
				return `<${_TNAME_}${_INL_}>${_STR_}</${_TNAME_}>`;
			}

			var echoTag = function(_TNAME_, _STR_){
				_OUTPUT_ += tag(_TNAME_, _STR_);
			}

			var echoTagStyle = function(_TNAME_, _INL_, _STR_){
				_OUTPUT_ += tag_style(_TNAME_, _INL_, _STR_);
			}


			///////////////////////////////////////////////////////////////////////////////////////////

			var br = function(num){
				for(var i=0; i<num;i++){
					_OUTPUT_ += "<br />\n";
				}
			}

			var macro = function(){
				return tag_style("span", 'style="font-style: italic; color: #71717A";', "[MACRO] ");
			}

			var constant = function(str){
				return tag_style("span", 'style="font-style: italic; color: #D4D4D8";', str);
			}

			var list = function(indentation, items){
				var output = "<ul style='font-size: 17px; padding-left: " + indentation + "px;'>";

				items.forEach((item)=>{
					output += "<li>" + item + "</li>";
				});

				return output + "</ul>";
			}


			var red    = function(str){ return tag_style("span", "style='color: #f92472;'", str); };
			var orange = function(str){ return tag_style("span", "style='color: #fd9622;'", str); };
			var yellow = function(str){ return tag_style("span", "style='color: #e7db74;'", str); };
			var green  = function(str){ return tag_style("span", "style='color: #a6e22b;'", str); };
			var cyan   = function(str){ return tag_style("span", "style='color: #67d8ef;'", str); };
			var purple = function(str){ return tag_style("span", "style='color: #ac80ff;'", str); };
			var gray   = function(str){ return tag_style("span", "style='color: #74705d;'", str); };
			var white  = function(str){ return tag_style("span", "style='color: #f8f8f2;'", str); };

			//////////////////////////////////////////////////////////////

			new Token("( |\r)", (str)=>{return str;});
			new Token("\n", (str)=>{return "<br />";});
			new Token("\t", (str)=>{
				return "<div style='display: inline-block; width:27px;'></div>";
			});
			new Token("//" + regex.endl, gray);

			new Token(regex.doubleQuotedString, yellow);
			new Token(regex.capture("<", ">"), (str)=>{
				return yellow("&lt;" + str.substring(1, str.length-1) + "&gt;");
			});
			new Token(regex.number, purple);

			new Token("(int|char|float|bool|unsigned|class|struct|void)", (str)=>{
				return tag_style("span", "style='color: #67d8ef; font-style: italic;'", str);
			});
			new Token("(public|private|protected|override|virtual|return|static|#include|new)", red);
			new Token("(operator)", green);

			new Token("(\\+|=|<<|\\*|&)", red);

			new Token("(" + regex.letter + "|_|~)(" + regex.letter + "|_|" + regex.digit + ")*\\(", (str)=>{
				return cyan(str.substring(0, str.length-1)) + white("(");
			});
			new Token("(" + regex.letter + "|_)(" + regex.letter + "|_|" + regex.digit + ")*", white);

			//////////////////////////////////////////////////////////////

			var param = function(type, name){
				return {
					type: type,
					name: name,
				}
			}


			var create_func = function(name, params, description){
				var style = "style='";
					style += "font-size: 17px;";
					style += "padding-left: "+indentation+"px;";
				style +="'";

				var params_str = "";
				for(var i=0; i<params.length;i++){
					var type = params[i].type.split(" ");
					type.forEach((t)=>{
						if(["int", "bool"].includes(t)){ //blue
							params_str += cyan(t);
						}else if(t == "cout"){ //red
							params_str += red("cout");
						}else{
							params_str = t;
						}

						params_str += " ";
					});


					params_str += orange(params[i].name);

					if(i < params.length - 1){
						params_str += ", ";
					}
				}

				var name_title = green(name) + "(" + params_str + ")";

				echoTagStyle("p", style, tag("code", name_title));


				var style = "style='";
					style += "font-size: 17px;";
					style += "padding-left: " + (indentation + 80) + "px;";
				style +="'";

				echoTagStyle("p", style, description);
			}



			var code = function(script){
				// echoTagStyle("div", "class='code'", script);
				var output = "";

				while(script.length > 0){
					var found = false;
					for(var i=0; i<tokens.length;i++){
						var token = tokens[i];

						var match = token.regex.exec(script);
						if(match != null){
							output += token.action(match[0]);
							script = script.replace(match[0], "");
							found = true;
							break;
						}
					}
					if(!found){
						output += script[0];
						script = script.replace(script[0], "");
					}
				}

				echoTagStyle("div", "class='code'", output);

			}

			var n = function(){
				return "<br />";
			}
			var t = function(num){
				var output = "";
				for(var i=0; i<num;i++){
					output += "&emsp;&emsp;";
				}
				return output;
			}



			///////////////////////////////////////////////////////////////////////////////////////////


			eval(_STRING_);
		}else{
			_OUTPUT_ += _FILE_[_CURSOR_];
		}
	}
	return _OUTPUT_;
}

module.exports = (_PATH_, data)=>{
	return _NODEHP_(_PATH_, data);
};