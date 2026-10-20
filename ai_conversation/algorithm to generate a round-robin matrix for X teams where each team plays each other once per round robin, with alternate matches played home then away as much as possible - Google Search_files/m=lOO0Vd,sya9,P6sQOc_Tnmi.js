loaded_h_0(function(_){var window=this;
_.v("lOO0Vd");
_.qob=new _.Jo(_.ZSa);
_.w();
var sob;sob=function(a){if(a.FRc){let b=Date.now()-a.vqe;return a.FRc(a.yIb+1,b)}return Math.random()*Math.min(a.o1d*Math.pow(a.bec,a.yIb),a.A9d)};_.tob=function(a){if(!a.f6a())throw Error("Ye`"+a.Ilb);++a.yIb;a.aec=sob(a)};_.uob=class{constructor(a,b,c,d,e,f){this.Ilb=a;this.o1d=b;this.bec=c;this.A9d=d;this.hje=e;this.FRc=f||null;this.vqe=Date.now();this.yIb=0;this.aec=sob(this)}Loc(){return this.yIb}f6a(a){return this.yIb>=this.Ilb?!1:a!=null?!!this.hje[a]:!0}};
_.v("P6sQOc");
var vob=function(a){var b={};_.Ka(a.Aa(),e=>{b[e]=!0});var c=a.Ba(),d=a.Ca();return new _.uob(a.wa(),_.Ee(c.getSeconds())*1E3,a.oa(),_.Ee(d.getSeconds())*1E3,b)},wob=new _.ar("retryConfigOverrides"),xob=function(a,b,c,d){return c.then(e=>e,e=>{if(e instanceof _.wya){if(!e.status||!d.f6a(_.Je(e.status,1)))throw e;}else if("function"==typeof _.Fkb&&e instanceof _.Fkb)switch(e.ka){case 103:case 7:case 10:case 101:case 105:case 408:case 425:case 429:case 502:case 503:case 504:break;default:throw e;}if(d&&
!d.f6a())return _.Lh(e);var f=d.aec;return(new _.Vg(g=>{setTimeout(g,f)})).then(()=>{_.tob(d);var g=d.Loc();b=b.Gv(_.zXa,g);return xob(a,b,a.fetch(b),d)})})};
_.jg(class{constructor(){this.ka=_.Uf(_.pob);this.oa=_.Uf(_.qob);this.logger=null;var a=_.Uf(_.hjb);this.fetch=a.fetch.bind(a)}M5a(a,b){if(this.oa.getType(a.qr())!==1)return new _.ljb(a,null,0);var c=this.ka.policy,d=_.cr(a,wob),e=null;if(d){e={};if(d.AIb)for(var f of d.AIb)e[f]=!0;else if(c)for(var g of c.Aa())e[g]=!0;f=1;g=0;let h=Infinity,k=2;if(c){f=c.wa()||f;let l=c.Ia()?.getSeconds();g=_.Ee(c.Ka().getSeconds())*1E3;h=l!=null?_.Ee(l)*1E3:h;k=c.oa()||k}e=new _.uob(d.maxAttempts??f,d.Zsc??g,d.Wdb??
k,d.Nxc??h,e,d.FFd)}else c&&(e=vob(c));e&&e.f6a()?(b=xob(this,a,b,e),a=new _.ljb(a,b,2)):a=new _.ljb(a,null,0);return a}},_.rob);
_.w();
});
// Google Inc.
