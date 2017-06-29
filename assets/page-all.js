/**
 * JS kod, ktery se vklada na vsechny stranky
 * - kod je rozdelen do 5 hlavnich funkci:
 *   bindDocumentReady() - pro pevny obsah, ktery se nevklada dynamicky pomoci ajaxu apod.
 *   bindByScope (scope) - pro pevny i dynamicky obsah vkladany ajaxem
 *   bindWindowLoad()    - pro JS, ktery je zavisly na nacteni cele stranky vcetne obrazku
 *   bindWindowResize()  - pro JS, ktery se ma zavolat po nacteni cele stranky a po zmene sirky okna
 *   bindResponse(), bindResponsePhone() ... bindResponseDesktop() - pro responzivni zmeny 
 *   + dalsich funkci a jejich pomocnych funkci, ktere se volaji v techto trech hlavnich
 * - do hlavnich funkci davat pouze a vyhradne jen volani funkci s parametry, nic jineho! Tzn. na vse psat samostatnou funkci 
 *   a dat pripadne ke kontrole programatorovi
 */

/**
 * Zakladni volani pri document ready
 * - NEMENIT
 */
$(document).ready(function() {
	bindDocumentReady();
	bindByScope();
	bindResponse();
});

/**
 * Zakladni volani pri window load
 * - NEMENIT
 */
$(window).on('load', function() {
	bindWindowLoad();
});

/**
 * Zakladni volani pri resize
 * - pokud se neaktualizoval viewport_size_code (nezmenilo se zarizeni),
 *   tak priznak zmeny viewport_size_code_changed nastavime na false
 * - NEMENIT
 */
$(window).on('resize', function() {
	if (viewport_size_code_checked) {
		viewport_size_code_checked = false;
	} else {
		viewport_size_code_changed = false;
	}
	bindWindowResize();
});

/** 
 * Pro responzivni upravy obsahu
 * - NEMENIT
 */
function bindResponse() {
	if ($('body').hasClass('is-responsive')) {
		mediaCheck({
			media: '(max-width: 575px)',
			entry: function() {
				setSizeCodeVars('XS');
				bindResponseXS();
			}
		});
		mediaCheck({
			media: '(min-width: 576px) and (max-width: 767px)',
			entry: function() {
				setSizeCodeVars('SM');
				bindResponseSM();
			}
		});
		mediaCheck({
			media: '(min-width: 768px) and (max-width: 991px)',
			entry: function() {
				setSizeCodeVars('MD');
				bindResponseMD();
			}
		});
		mediaCheck({
			media: '(min-width: 992px) and (max-width: 1199px)',
			entry: function() {
				setSizeCodeVars('LG');
				bindResponseLG();
			}
		});
		mediaCheck({
			media: '(min-width: 1200px)',
			entry: function() {
				setSizeCodeVars('XL');
				bindResponseXL();
			}
		});
	}
}

/**
 * Nastavi promenne s kodem sirky
 * - XS, SM, MD, LG, XL a predchozi kod
 * - NEMENIT
 */
function setSizeCodeVars(size_code) {
	viewport_size_code_checked = true;
	if (typeof(viewport_size_code) == 'undefined') {
		viewport_size_code = size_code;
		viewport_size_code_previous = null;
	} else {
		viewport_size_code_previous = viewport_size_code;
		viewport_size_code = size_code;
	}
	if (viewport_size_code_previous != viewport_size_code) {
		viewport_size_code_changed = true;
	} else {
		viewport_size_code_changed = false;
	}
}


/* =============
 * FUNKCE NIZE EDITOVAT VIZ KOMENTARE K FUNKCIM 
 * =============
 */

/**
 * zavola se pri document ready a nevola se na vlozene casti kodu ajaxem (k tomu slouzi funkce bindByScope nize)
 * !!! vkladat pouze volani funkci s parametry, nic jineho !!!
 */
function bindDocumentReady() {
    
}

/**
 * zavola se na cely dokument po nacteni stranky  + na casti HTML dle scope, ktere se muze vkladat i ajaxem pozdeji do stranky (napr. v shopu kosik nebo formulare)
 * !!! vkladat pouze volani funkci se scope, nic jineho !!!
 */
function bindByScope(scope) {
    
}

/**
 * zavola se az po nacteni vseho vcetne obrazku 
 * - patri sem JS, ktery pracuje s sirkami a vyskami elementu po nacteni obrazku bez zadaneho rozmeru apod.
 * - zavola se nekdy az po nekolika sekundach, az jsou vsechny obrazky nacteny, nejlepsi se je tomu vyhnout zadanim rozmeru obrazku a pouzitim bindDocumentReady/bindByScope vyse
 * !!! vkladat pouze volani funkci s parametry, nic jineho !!!
 */
function bindWindowLoad() {
    
}

/**
 * JS volany pri zmene sirky
 */
function bindWindowResize() {
}

/**
 * zavola se pro extra small displeje
 */
function bindResponseXS() {
    
}

/**
 * zavola se pro small displeje
 */
function bindResponseSM() {
    
}

/**
 * zavola se pro middle displeje
 */
function bindResponseMD() {
    
}

/**
 * zavola se pro large displeje
 */
function bindResponseLG() {
	
}

/**
 * zavola se pro extra large displeje
 */
function bindResponseXL() {
	
}


/* =============
 * funkce volane z hlavnich funkci bindXXX
 * !!! na kazdou funcionalitu samostatnou funkci, nedelat spagetovy kod, nelepit vic veci do jedne funkce !!!
 * =============
 */

/**
 * funkce na zpozdeni scriptu
 */
var delay = (function() {
	var timer = 0;
	return function(callback, ms) {
		clearTimeout(timer);
		timer = setTimeout(callback, ms);
	};
})();