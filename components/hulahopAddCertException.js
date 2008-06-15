Components.utils.import("resource://gre/modules/XPCOMUtils.jsm");

function AddCertException() { };

AddCertException.prototype = {
    classDescription: "AddCertException XPCOM Component",
    classID:          Components.ID("{4117128f-0e50-4863-b7a7-40a853451cc0}"),
    contractID:       "@sugarlabs.org/add-cert-exception;1",
    QueryInterface: XPCOMUtils.generateQI([Components.interfaces.hulahopAddCertException]),
    showDialog: function(window) {
        var params = { location : window.location.href };
        window.openDialog('chrome://pippki/content/exceptionDialog.xul',
                          '', 'chrome,centerscreen,modal', params);
    }
}

var components = [AddCertException];
function NSGetModule(compMgr, fileSpec) {
    return XPCOMUtils.generateModule(components);
}

