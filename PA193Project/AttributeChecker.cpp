#include "stdafx.h"
#include "AttributeChecker.h"

string Valid_Attribute_List[119] = { "abbr","accept-charset","accept","accesskey","action","align","alink","alt","archive","axis","background","bgcolor","border","cellpadding","cellspacing","char","charoff","charset","checked","cite","class","classid","clear","code","codebase","codetype","color","cols","colspan","compact","content","coords","data","datetime","declare","defer","dir","disabled","enctype","face","for","frame","frameborder","headers","height","href","hreflang","hspace","http-equiv","id","ismap","label","lang","language","link","longdesc","marginheight","marginwidth","maxlength","media","method","multiple","name","nohref","noresize","noshade","nowrap","object","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload","profile","prompt","readonly","rel","rev","rows","rowspan","rules","scheme","scope","scrolling","selected","shape","size","span","src","standby","start","style","summary","tabindex","target","text","title","type","usemap","valign","value","valuetype","version","vlink","vspace","width" };

bool isValidAttribute(string attribute) {
	if (attribute.length() > 15) {
		return false;
	}
	//There are 116 valid attributes (http://www.w3.org/TR/html4/index/attributes.html)
	//string Valid_Attribute_List[119] = { "abbr","accept-charset","accept","accesskey","action","align","alink","alt","archive","axis","background","bgcolor","border","cellpadding","cellspacing","char","charoff","charset","checked","cite","class","classid","clear","code","codebase","codetype","color","cols","colspan","compact","content","coords","data","datetime","declare","defer","dir","disabled","enctype","face","for","frame","frameborder","headers","height","href","hreflang","hspace","http-equiv","id","ismap","label","lang","language","link","longdesc","marginheight","marginwidth","maxlength","media","method","multiple","name","nohref","noresize","noshade","nowrap","object","onblur","onchange","onclick","ondblclick","onfocus","onkeydown","onkeypress","onkeyup","onload","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onreset","onselect","onsubmit","onunload","profile","prompt","readonly","rel","rev","rows","rowspan","rules","scheme","scope","scrolling","selected","shape","size","span","src","standby","start","style","summary","tabindex","target","text","title","type","usemap","valign","value","valuetype","version","vlink","vspace","width" };
	for (int k = 0; k < 119; k++)
	{
		if (attribute.compare(Valid_Attribute_List[k]) == 0)
		{
			return true;
		}

	}
	return false;
}
