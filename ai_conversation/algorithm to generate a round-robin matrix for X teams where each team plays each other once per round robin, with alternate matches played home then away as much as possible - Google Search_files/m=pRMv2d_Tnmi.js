"use strict";loaded_g_0(function(_){var window=this;
_.n("pRMv2d");
_.w3a(_.nC,class extends _.x3a{static jb(){return{}}constructor(){super();this.places=new Map;this.na=null;this.Ba=new Set;this.za=new Set;this.ya=new Set;this.wa=new Set;this.ka=!1;this.ab=_.bv(this.mb,100,this)}va(a){return this.places.get(a)}ba(){return[...this.places.values()]}Fa(){this.ka=!1;this.places.clear()}ha(...a){var b=!1;for(let c of a){let d=this.places.get(c.mid);d?(!d.latLng&&c.latLng&&(d.latLng=c.latLng,b=!0),!d.fprint&&c.fprint&&(d.fprint=c.fprint,b=!0),!d.categoryName&&c.categoryName&&
(d.categoryName=c.categoryName,b=!0),a=Object.fromEntries(Object.entries(c).filter(([e,f])=>!d[e]&&(f!==void 0||f!==""))),Object.assign(d,a)):this.places.set(c.mid,Object.assign({},c));b=!0}b&&this.ab()}aa(a){this.na=a?Object.assign({},a):null;for(let b of this.za)b(this.na)}Ga(a){this.za.add(a);this.na&&a(this.na)}Pa(a){this.Ba.add(a);var b=this.ba();b.length&&a(b)}Ka(a){this.ya.add(a);this.ka&&this.ba().filter(b=>!!b.latLng).length===0&&a()}Bb(a){this.wa.add(a);this.ka&&a()}oa(){this.ka=!0;this.ba().filter(a=>
!!a.latLng).length===0&&this.ya.forEach(a=>void a());this.wa.forEach(a=>void a())}mb(){var a=this.ba();if(a.length)for(let b of this.Ba)b(a)}});
_.J();
});
// Google Inc.
