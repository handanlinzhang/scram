variable
   bool  r1, g1, g2, g3, g4, g5, g15, g13, g6, g7, g14, g8, g9, g12, g10, g11, g208, g288, g260, g286, g198, g287, g218, g289, g16, g17, e1, e213, e214, e215, g18, g19, e210, e211, e212, g20, e2, g21, g22, g23, e3, e219, e220, e221, g24, g25, e216, e217, e218, g26, e4, g27, g28, e5, e6, g29, e222, e223, e224, e225, g30, e7, e8, e9, e10, e11, e12, e13, e14, g31, e15, e16, e17, e18, g32, e19, e20, g33, e21, g34, e22, e23, e24, e25, e26, e27, e28, e29, g35, e30, e31, e32, e33, g36, e34, e35, g37, e36, g38, g39, g40, g41, e37, g42, g43, e42, e43, e44, e45, e46, e47, e48, e49, g44, g45, g46, g47, e38, e39, e40, e41, e176, e177, e178, e179, g48, g49, g50, g51, e50, g52, g53, e53, e54, e55, e56, g54, g55, e51, e52, g56, g57, g58, g59, e57, g60, g61, e61, e62, g62, g63, e58, e59, e60, g64, g65, g66, g67, e63, g68, g69, e66, g70, g71, e64, e65, g72, e67, e68, e69, e70, e71, e72, e73, e74, g73, e75, e76, e77, e78, g74, e79, e80, g75, e81, g76, g77, g78, g79, e82, g80, g81, e83, e84, g82, g83, g84, g85, g86, g87, e85, g88, g89, e86, g90, g91, g92, e87, e88, e89, e90, e91, e92, e93, e94, g93, e95, e96, e97, e98, e99, e100, e101, e102, g94, g95, e103, g96, g97, e104, e105, e106, g98, g99, e109, e107, e108, e110, e111, g100, e112, g101, g102, e275, e276, e180, e181, e182, e183, e184, e185, g103, e113, e114, e115, e116, g104, e117, e118, e119, e120, g105, g106, e121, g107, g108, e122, e123, e124, g109, g110, e127, e125, e126, e128, e129, g111, e130, g112, g113, e253, e254, e186, e187, e188, e189, e190, e191, g114, e131, e132, g115, e133, e134, g116, g117, e135, g118, g119, g120, e255, e256, g238, g239, e192, e193, e194, e195, e196, e197, g121, g122, e136, g123, g124, e137, e138, g125, e139, e140, g126, e141, g127, e142, g128, g129, e143, g130, g131, g132, e257, e258, g240, g241, e198, e199, e200, e201, e202, e203, g133, g134, e144, g135, g136, e145, e146, g137, e147, e148, g138, g139, g140, g141, e149, e150, g142, e151, g143, g144, g145, g146, e152, e153, g147, e154, g148, g149, e155, g150, g151, e156, e157, g152, g153, g154, g155, e158, e159, g156, g157, e160, g158, g159, e161, e162, g160, g161, g162, g163, g164, g165, g166, g167, e163, e164, g168, g169, g170, g171, g172, g173, g174, g175, e165, e166, g176, g177, g178, g179, g180, g181, g182, g183, g184, e171, e172, e167, e168, e169, e170, g185, g186, e204, e205, e206, g187, g188, g189, g190, g191, e175, e173, e174, g192, g193, e207, e208, e209, g194, e226, e227, e228, e229, e230, e231, g195, e232, e233, e234, e235, e236, e237, g196, e238, e239, e240, e241, e242, e243, g197, e244, e245, e246, e247, e248, e249, g199, g200, g201, g202, e250, g207, g232, g252, g242, g258, g203, g204, g205, g206, g234, g244, g271, g272, g209, g210, g211, g212, e251, g217, g248, g257, g235, g251, g213, g214, g215, g216, g250, g237, g276, g277, g219, g220, g221, g222, e252, g227, g253, g259, g223, g224, g225, g226, g281, g282, g228, g229, g230, g231, g233, e259, g236, e260, g243, e261, g245, g246, e262, e263, g247, g249, e264, e265, e266, e267, e268, g254, g255, e269, e270, g256, e271, e272, e273, e274, g261, g262, g265, e277, g263, g264, g266, g267, e278, e279, e280, e281, g268, g269, e282, e283, e284, g270, e285, e286, e287, e288, e289, e290, e291, e292, e293, e294, e295, g273, g274, g275, e296, e297, e298, e299, e300, e301, e302, e303, g278, g279, e304, g280, e305, e306, e307, e308, e309, g283, g284, g285, e310, e311;

rule
   (r1==g1);
   (g1==(g2 || g3));
   (g2==(g4 || g5));
   (g3==(g15 && g13));
   (g4==(g6 || g7));
   (g5==(g14 && g13));
   (g6==(g8 && g9));
   (g7==(g12 && g13));
   (g8==(g10 && g11));
   (g9==(g208 || g288));
   (g10==(g260 || g286));
   (g11==(g198 || g287));
   (g12==(g10 && g11));
   (g13==(g218 || g289));
   (g14==(g10 && g9));
   (g15==(g11 && g9));
   (g16==(g17 && e1));
   (g17==(e213 || e214 || e215));
   (g18==(g16 || g19));
   (g19==(e210 || e211 || e212));
   (g20==(g19 && e2));
   (g21==(g20 || g17));
   (g22==(g23 && e3));
   (g23==(e219 || e220 || e221));
   (g24==(g22 || g25));
   (g25==(e216 || e217 || e218));
   (g26==(g25 && e4));
   (g27==(g26 || g23));
   (g28==(e5 || e6 || g29));
   (g29==(e222 || e223 || e224 || e225));
   (g30==(e7 || e8 || e9 || e10 || e11 || e12 || e13 || e14));
   (g31==(e15 || e8 || e9 || e11 || e16 || e17 || e18 || e13));
   (g32==(e19 || e8 || e10 || e11 || e16 || e17 || e20 || e14));
   (g33==(e21 || e8 || e9 || e10 || e16 || e12 || e20 || e18));
   (g34==(e22 || e23 || e24 || e25 || e26 || e27 || e28 || e29));
   (g35==(e30 || e23 || e24 || e26 || e31 || e32 || e33 || e28));
   (g36==(e34 || e23 || e25 || e26 || e31 || e32 || e35 || e29));
   (g37==(e36 || e23 || e24 || e25 || e31 || e27 || e35 || e33));
   (g38==(g39 || g40));
   (g39==(g41 || e37));
   (g40==(g42 && g43));
   (g41==(e42 || e43 || e44 || e45 || e46 || e47 || e48 || e49));
   (g42==(g44 && g45));
   (g43==(g46 && g47));
   (g44==(e38 || e39));
   (g45==(e40 || e41));
   (g46==(e176 || e177));
   (g47==(e178 || e179));
   (g48==(g49 || g50));
   (g49==(g51 || e50));
   (g50==(g52 && g53));
   (g51==(e53 || e43 || e44 || e46 || e54 || e55 || e56 || e48));
   (g52==(g54 && g55));
   (g53==(g46 && g47));
   (g54==(e38 || e51));
   (g55==(e40 || e52));
   (g56==(g57 || g58));
   (g57==(g59 || e57));
   (g58==(g60 && g61));
   (g59==(e61 || e43 || e45 || e46 || e54 || e55 || e62 || e49));
   (g60==(g62 && g63));
   (g61==(g46 && g47));
   (g62==(e58 || e59));
   (g63==(e40 || e60));
   (g64==(g65 || g66));
   (g65==(g67 || e63));
   (g66==(g68 && g69));
   (g67==(e66 || e43 || e44 || e45 || e54 || e47 || e62 || e56));
   (g68==(g70 && g71));
   (g69==(g46 && g47));
   (g70==(e58 || e64));
   (g71==(e40 || e65));
   (g72==(e67 || e68 || e69 || e70 || e71 || e72 || e73 || e74));
   (g73==(e75 || e68 || e69 || e71 || e76 || e77 || e78 || e73));
   (g74==(e79 || e68 || e70 || e71 || e76 || e77 || e80 || e74));
   (g75==(e81 || e68 || e69 || e70 || e76 || e72 || e80 || e78));
   (g76==(g77 || g78));
   (g77==(g79 || e82));
   (g78==(g80 && g81));
   (g79==(e83 || e84));
   (g80==(g82 && g83));
   (g81==(g46 && g47));
   (g82==(e38 || e51));
   (g83==(e40 || e52));
   (g84==(g85 || g86));
   (g85==(g87 || e85));
   (g86==(g88 && g89));
   (g87==(e86 || e84));
   (g88==(g90 && g91));
   (g89==(g46 && g47));
   (g90==(e58 || e64));
   (g91==(e40 || e65));
   (g92==(e87 || e88 || e89 || e90 || e91 || e92 || e93 || e94));
   (g93==(e95 || e96 || e97 || e98 || e99 || e100 || e101 || e102));
   (g94==(g95 || e103 || g96 || g97 || e104 || g93 || g92));
   (g95==(e105 || e106));
   (g96==(g98 && g46));
   (g97==(g99 || e109));
   (g98==(e107 || e108));
   (g99==(e110 || e111));
   (g100==(e112 || g94 || g101 || g102));
   (g101==(e275 || e276));
   (g102==(e180 || e181 || e182 || e183 || e184 || e185));
   (g103==(e113 || e88 || e89 || e91 || e114 || e115 || e116 || e93));
   (g104==(e117 || e96 || e97 || e99 || e118 || e119 || e120 || e101));
   (g105==(g106 || e121 || g107 || g108 || e122 || g104 || g103));
   (g106==(e123 || e124));
   (g107==(g109 && g47));
   (g108==(g110 || e127));
   (g109==(e125 || e126));
   (g110==(e128 || e129));
   (g111==(e130 || g105 || g112 || g113));
   (g112==(e253 || e254));
   (g113==(e186 || e187 || e188 || e189 || e190 || e191));
   (g114==(e131 || e88 || e90 || e91 || e114 || e115 || e132 || e94));
   (g115==(e133 || e96 || e98 || e99 || e118 || e119 || e134 || e102));
   (g116==(g115 || g114));
   (g117==(e135 || g116 || g118 || g119 || g120));
   (g118==(e255 || e256));
   (g119==(g238 || g239));
   (g120==(e192 || e193 || e194 || e195 || e196 || e197));
   (g121==(g122 && e136));
   (g122==(g123 || g124));
   (g123==(e137 || e138));
   (g124==(g125 && g46));
   (g125==(e139 || e140));
   (g126==(e141 || e88 || e89 || e90 || e114 || e92 || e132 || e116));
   (g127==(e142 || e96 || e97 || e98 || e118 || e100 || e134 || e120));
   (g128==(g127 || g126));
   (g129==(e143 || g128 || g130 || g131 || g132));
   (g130==(e257 || e258));
   (g131==(g240 || g241));
   (g132==(e198 || e199 || e200 || e201 || e202 || e203));
   (g133==(g134 && e144));
   (g134==(g135 || g136));
   (g135==(e145 || e146));
   (g136==(g137 && g47));
   (g137==(e147 || e148));
   (g138==(g139 && e136));
   (g139==(g140 || g141));
   (g140==(e149 || e150));
   (g141==(g142 && g46));
   (g142==(e139 || e151));
   (g143==(g144 && e144));
   (g144==(g145 || g146));
   (g145==(e152 || e153));
   (g146==(g147 && g47));
   (g147==(e147 || e154));
   (g148==(g149 && e155));
   (g149==(g150 || g151));
   (g150==(e156 || e157));
   (g151==(g152 && g153));
   (g152==(g154 && g155));
   (g153==(g46 && g47));
   (g154==(e139 || e158));
   (g155==(e147 || e159));
   (g156==(g157 && e160));
   (g157==(g158 || g159));
   (g158==(e161 || e162));
   (g159==(g160 && g161));
   (g160==(g162 && g163));
   (g161==(g46 && g47));
   (g162==(e139 || e158));
   (g163==(e147 || e159));
   (g164==(g165 && e155));
   (g165==(g166 || g167));
   (g166==(e163 || e164));
   (g167==(g168 && g169));
   (g168==(g170 && g171));
   (g169==(g46 && g47));
   (g170==(e139 || e140));
   (g171==(e147 || e148));
   (g172==(g173 && e160));
   (g173==(g174 || g175));
   (g174==(e165 || e166));
   (g175==(g176 && g177));
   (g176==(g178 && g179));
   (g177==(g46 && g47));
   (g178==(e139 || e140));
   (g179==(e147 || e148));
   (g180==(g181 || g182));
   (g181==(g183 || g184));
   (g182==(e171 || e172));
   (g183==(e167 || e168));
   (g184==(e169 || e170));
   (g185==(g180 || g186));
   (g186==(e204 || e205 || e206));
   (g187==(g188 || g189));
   (g188==(g190 || g191));
   (g189==(e175 || e172));
   (g190==(e173 || e168));
   (g191==(e174 || e170));
   (g192==(g187 || g193));
   (g193==(e207 || e208 || e209));
   (g194==(e226 || e227 || e183 || e195 || e228 || e229 || e230 || e231));
   (g195==(e232 || e233 || e184 || e196 || e234 || e235 || e236 || e237));
   (g196==(e238 || e239 || e189 || e201 || e240 || e241 || e242 || e243));
   (g197==(e244 || e245 || e190 || e202 || e246 || e247 || e248 || e249));
   (g198==(g199 || g200));
   (g199==(g201 && g202));
   (g200==(g31 || g35 || g195 || e250 || g207));
   (g201==(g232 || g252));
   (g202==(g242 || g258));
   (g203==(g204 || g200));
   (g204==(g205 || g206));
   (g205==(g234 || g252));
   (g206==(g244 || g258));
   (g207==(g271 || g272));
   (g208==(g209 || g210));
   (g209==(g211 && g212));
   (g210==(g32 || g36 || g196 || e251 || g217));
   (g211==(g248 || g257));
   (g212==(g235 || g251));
   (g213==(g214 || g210));
   (g214==(g215 || g216));
   (g215==(g250 || g257));
   (g216==(g237 || g251));
   (g217==(g276 || g277));
   (g218==(g219 || g220));
   (g219==(g221 && g222));
   (g220==(g33 || g37 || g197 || e252 || g227));
   (g221==(g235 || g253));
   (g222==(g248 || g259));
   (g223==(g224 || g220));
   (g224==(g225 || g226));
   (g225==(g237 || g253));
   (g226==(g250 || g259));
   (g227==(g281 || g282));
   (g228==(g38 || g18 || g72));
   (g229==(g48 || g21 || g73));
   (g230==(g74 || g56 || g24));
   (g231==(g75 || g64 || g27));
   (g232==(g28 || g233));
   (g233==(g100 || e259));
   (g234==(g28 || g233));
   (g235==(g28 || g236));
   (g236==(g111 || e260));
   (g237==(g28 || g236));
   (g238==(g228 && g229));
   (g239==(g76 || g185));
   (g240==(g230 && g231));
   (g241==(g84 || g192));
   (g242==(g28 || g243));
   (g243==(g117 || e261));
   (g244==(g28 || g243));
   (g245==(g232 || g246));
   (g246==(g121 || e262 || e263));
   (g247==(g234 || g246));
   (g248==(g28 || g249));
   (g249==(g129 || e264));
   (g250==(g28 || g249));
   (g251==(g133 || e265 || e266));
   (g252==(g138 || e267 || e263));
   (g253==(g143 || e268 || e266));
   (g254==(g242 || g255));
   (g255==(g148 || e269 || e270));
   (g256==(g244 || g255));
   (g257==(g156 || e271 || e272));
   (g258==(g164 || e273 || e270));
   (g259==(g172 || e274 || e272));
   (g260==(g261 || g262));
   (g261==(g254 && g245));
   (g262==(g265 || e277 || g194 || g34 || g30));
   (g263==(g264 || g262));
   (g264==(g256 || g247));
   (g265==(g266 || g267));
   (g266==(e278 || e279 || e280));
   (g267==(e281 || g268));
   (g268==(g269 && e282));
   (g269==(e283 || e284 || g270 || e285));
   (g270==(e286 || e287 || e288 || e289 || e290 || e291 || e292 || e293));
   (g271==(e278 || e279 || e294));
   (g272==(e295 || g273));
   (g273==(g274 && e282));
   (g274==(e283 || e284 || g275 || e296));
   (g275==(e297 || e287 || e288 || e290 || e298 || e299 || e300 || e292));
   (g276==(e278 || e301 || e302));
   (g277==(e303 || g278));
   (g278==(g279 && e282));
   (g279==(e283 || e304 || g280 || e305));
   (g280==(e306 || e287 || e289 || e290 || e298 || e299 || e307 || e293));
   (g281==(e278 || e301 || e308));
   (g282==(e309 || g283));
   (g283==(g284 && e282));
   (g284==(e283 || e304 || g285 || e310));
   (g285==(e311 || e287 || e288 || e289 || e298 || e291 || e307 || e300));
   (g286==(g263 && g195));
   (g287==(g203 && g194));
   (g288==(g213 && g197));
   (g289==(g223 && g196));