#ifndef IBE_TIME
#define IBE_TIME

typedef struct { 
  const char* tz;
  short offset;
} TimeOffset;

const TimeOffset _to[] {
  {"Africa/Abidjan", (short)0},
  {"Africa/Accra", (short)10800},
  {"Africa/Addis_Ababa", (short)10800},
  {"Africa/Algiers", (short)3600},
  {"Africa/Asmara", (short)10800},
  {"Africa/Bamako", (short)0},
  {"Africa/Bangui", (short)3600},
  {"Africa/Banjul", (short)0},
  {"Africa/Bissau", (short)0},
  {"Africa/Blantyre", (short)7200},
  {"Africa/Brazzaville", (short)3600},
  {"Africa/Bujumbura", (short)7200},
  {"Africa/Cairo", (short)7200},
  {"Africa/Casablanca", (short)3600},
  {"Africa/Ceuta", (short)7200},
  {"Africa/Conakry", (short)0},
  {"Africa/Dakar", (short)0},
  {"Africa/Dar_es_Salaam", (short)10800},
  {"Africa/Djibouti", (short)10800},
  {"Africa/Douala", (short)3600},
  {"Africa/El_Aaiun", (short)3600},
  {"Africa/Freetown", (short)0},
  {"Africa/Gaborone", (short)7200},
  {"Africa/Harare", (short)7200},
  {"Africa/Johannesburg", (short)7200},
  {"Africa/Juba", (short)7200},
  {"Africa/Kampala", (short)10800},
  {"Africa/Khartoum", (short)7200},
  {"Africa/Kigali", (short)7200},
  {"Africa/Kinshasa", (short)3600},
  {"Africa/Lagos", (short)3600},
  {"Africa/Libreville", (short)3600},
  {"Africa/Lome", (short)0},
  {"Africa/Luanda", (short)3600},
  {"Africa/Lubumbashi", (short)7200},
  {"Africa/Lusaka", (short)7200},
  {"Africa/Malabo", (short)3600},
  {"Africa/Maputo", (short)7200},
  {"Africa/Maseru ", (short)7200},
  {"Africa/Mbabane", (short)7200},
  {"Africa/Mogadishu", (short)10800},
  {"Africa/Monrovia", (short)0},
  {"Africa/Nairobi", (short)10800},
  {"Africa/Ndjamena", (short)3600},
  {"Africa/Niamey", (short)3600},
  {"Africa/Nouakchott", (short)0},
  {"Africa/Ouagadougou", (short)0},
  {"Africa/Porto-Novo", (short)3600},
  {"Africa/Sao_Tome", (short)0},
  {"Africa/Timbuktu", (short)0},
  {"Africa/Tripoli", (short)7200},
  {"Africa/Tunis", (short)3600},
  {"Africa/Windhoek", (short)7200},
  {"America/Adak", (short)-32400},
  {"America/Anchorage", (short)-28800},
  {"America/Anguilla", (short)-14400},
  {"America/Antigua", (short)-14400},
  {"America/Araguaina", (short)-10800},
  {"America/Argentina/Buenos_Aires", (short)-10800},
  {"America/Argentina/Catamarca", (short)-10800},
  {"America/Argentina/ComodRivadavia", (short)-10800},
  {"America/Argentina/Cordoba", (short)-10800},
  {"America/Argentina/Jujuy", (short)-10800},
  {"America/Argentina/La_Rioja", (short)-10800},
  {"America/Argentina/Mendoza", (short)-10800},
  {"America/Argentina/Rio_Gallegos", (short)-10800},
  {"America/Argentina/Salta", (short)-10800},
  {"America/Argentina/San_Juan", (short)-10800},
  {"America/Argentina/San_Luis", (short)-3600},
  {"America/Argentina/Tucuman", (short)-10800},
  {"America/Argentina/Ushuaia", (short)-10800},
  {"America/Aruba", (short)-14400},
  {"America/Asuncion", (short)-14400},
  {"America/Atikokan", (short)-18000},
  {"America/Atka", (short)-32400},
  {"America/Bahia", (short)-10800},
  {"America/Bahia_Banderas", (short)-18000},
  {"America/Barbados", (short)-14400},
  {"America/Belem", (short)-10800},
  {"America/Belize", (short)-21600},
  {"America/Blanc-Sablon", (short)-14400},
  {"America/Boa_Vista", (short)-14400},
  {"America/Bogota", (short)-18000},
  {"America/Boise", (short)-21600},
  {"America/Buenos_Aires", (short)-10800},
  {"America/Cambridge_Bay", (short)-21600},
  {"America/Campo_Grande", (short)-14400},
  {"America/Cancun", (short)-18000},
  {"America/Caracas", (short)-14400},
  {"America/Catamarca", (short)-10800},
  {"America/Cayenne", (short)-10800},
  {"America/Cayman", (short)-18000},
  {"America/Chicago", (short)-18000},
  {"America/Chihuahua", (short)-21600},
  {"America/Coral_Harbour", (short)-18000},
  {"America/Cordoba", (short)-10800},
  {"America/Costa_Rica", (short)-21600},
  {"America/Creston", (short)-25200},
  {"America/Cuiaba", (short)-14400},
  {"America/Curacao", (short)-14400},
  {"America/Danmarkshavn", (short)0},
  {"America/Dawson", (short)-25200},
  {"America/Dawson_Creek", (short)-25200},
  {"America/Denver", (short)-21600},
  {"America/Detroit", (short)-14400},
  {"America/Dominica", (short)-14400},
  {"America/Edmonton", (short)-21600},
  {"America/Eirunepe", (short)-18000},
  {"America/El_Salvador", (short)-21600},
  {"America/Ensenada", (short)-25200},
  {"America/Fort_Nelson", (short)-25200},
  {"America/Fort_Wayne", (short)-14400},
  {"America/Fortaleza", (short)-10800},
  {"America/Glace_Bay", (short)-10800},
  {"America/Godthab", (short)-7200},
  {"America/Goose_Bay", (short)-10800},
  {"America/Grand_Turk", (short)-14400},
  {"America/Grenada", (short)-14400},
  {"America/Guadeloupe", (short)-14400},
  {"America/Guatemala", (short)-21600},
  {"America/Guayaquil", (short)-18000},
  {"America/Guyana", (short)-14400},
  {"America/Halifax", (short)-10800},
  {"America/Havana", (short)-14400},
  {"America/Hermosillo", (short)-25200},
  {"America/Indiana/Indianapolis", (short)-14400},
  {"America/Indiana/Knox", (short)-18000},
  {"America/Indiana/Marengo", (short)-14400},
  {"America/Indiana/Petersburg", (short)-14400},
  {"America/Indiana/Tell_City", (short)-18000},
  {"America/Indiana/Vevay", (short)-14400},
  {"America/Indiana/Vincennes", (short)-14400},
  {"America/Indiana/Winamac", (short)-14400},
  {"America/Indianapolis", (short)-14400},
  {"America/Inuvik", (short)-21600},
  {"America/Iqaluit", (short)-14400},
  {"America/Jamaica", (short)-18000},
  {"America/Jujuy", (short)-10800},
  {"America/Juneau", (short)-28800},
  {"America/Kentucky/Louisville", (short)-14400},
  {"America/Kentucky/Monticello", (short)-14400},
  {"America/Knox_IN", (short)-18000},
  {"America/Kralendijk", (short)-14400},
  {"America/La_Paz", (short)-14400},
  {"America/Lima", (short)-18000},
  {"America/Los_Angeles", (short)-25200},
  {"America/Louisville", (short)-14400},
  {"America/Lower_Princes", (short)-14400},
  {"America/Maceio", (short)-10800},
  {"America/Managua", (short)-21600},
  {"America/Manaus", (short)-14400},
  {"America/Marigot", (short)-14400},
  {"America/Martinique", (short)-14400},
  {"America/Matamoros", (short)-18000},
  {"America/Mazatlan", (short)-21600},
  {"America/Mendoza", (short)-10800},
  {"America/Menominee", (short)-18000},
  {"America/Merida", (short)-18000},
  {"America/Metlakatla", (short)-28800},
  {"America/Mexico_City", (short)-18000},
  {"America/Miquelon", (short)-7200},
  {"America/Moncton", (short)-10800},
  {"America/Monterrey", (short)-18000},
  {"America/Montevideo", (short)-10800},
  {"America/Montreal", (short)-14400},
  {"America/Montserrat", (short)-14400},
  {"America/Nassau", (short)-14400},
  {"America/New_York", (short)-14400},
  {"America/Nipigon", (short)-14400},
  {"America/Nome", (short)-28800},
  {"America/Noronha", (short)-7200},
  {"America/North_Dakota/Beulah", (short)-18000},
  {"America/North_Dakota/Center", (short)-18000},
  {"America/North_Dakota/New_Salem", (short)-18000},
  {"America/Nuuk", (short)-7200},
  {"America/Ojinaga", (short)-21600},
  {"America/Panama", (short)-18000},
  {"America/Pangnirtung", (short)-14400},
  {"America/Paramaribo", (short)-10800},
  {"America/Phoenix", (short)-25200},
  {"America/Port-au-Prince", (short)-14400},
  {"America/Port_of_Spain", (short)-14400},
  {"America/Porto_Acre", (short)-18000},
  {"America/Porto_Velho", (short)-14400},
  {"America/Puerto_Rico", (short)14400},
  {"America/Punta_Arenas", (short)10800},
  {"America/Rainy_River", (short)-18000},
  {"America/Rankin_Inlet", (short)-18000},
  {"America/Recife", (short)-10800},
  {"America/Regina", (short)-21600},
  {"America/Resolute", (short)-18000},
  {"America/Rio_Branco", (short)-18000},
  {"America/Rosario", (short)-10800},
  {"America/Santa_Isabel", (short)-25200},
  {"America/Santarem", (short)-10800},
  {"America/Santiago", (short)-10800},
  {"America/Santo_Domingo", (short)-14400},
  {"America/Sao_Paulo", (short)-10800},
  {"America/Scoresbysund", (short)0},
  {"America/Shiprock", (short)-21600},
  {"America/Sitka", (short)-28800},
  {"America/St_Barthelemy", (short)-14400},
  {"America/St_Johns", (short)-9000},
  {"America/St_Kitts", (short)-14400},
  {"America/St_Lucia", (short)-14400},
  {"America/St_Thomas", (short)-14400},
  {"America/St_Vincent", (short)-14400},
  {"America/Swift_Current", (short)-21600},
  {"America/Tegucigalpa", (short)-21600},
  {"America/Thule", (short)-10800},
  {"America/Thunder_Bay", (short)-14400},
  {"America/Tijuana", (short)-25200},
  {"America/Toronto", (short)-14400},
  {"America/Tortola", (short)-14400},
  {"America/Vancouver", (short)-25200},
  {"America/Virgin", (short)-14400},
  {"America/Whitehorse", (short)-25200},
  {"America/Winnipeg", (short)-18000},
  {"America/Yakutat", (short)-28800},
  {"America/Yellowknife", (short)-21600},
  {"Antarctica/Casey", (short)39600},
  {"Antarctica/Davis", (short)25200},
  {"Antarctica/DumontDUrville", (short)36000},
  {"Antarctica/Macquarie", (short)36000},
  {"Antarctica/Mawson", (short)18000},
  {"Antarctica/McMurdo", (short)43200},
  {"Antarctica/Palmer", (short)-10800},
  {"Antarctica/Rothera", (short)-10800},
  {"Antarctica/South_Pole", (short)43200},
  {"Antarctica/Syowa", (short)10800},
  {"Antarctica/Troll", (short)7200},
  {"Antarctica/Vostok", (short)21600},
  {"Arctic/Longyearbyen", (short)7200},
  {"Asia/Aden", (short)10800},
  {"Asia/Almaty", (short)21600},
  {"Asia/Amman", (short)10800},
  {"Asia/Anadyr", (short)43200},
  {"Asia/Aqtau", (short)18000},
  {"Asia/Aqtobe", (short)18000},
  {"Asia/Ashgabat", (short)18000},
  {"Asia/Ashkhabad", (short)18000},
  {"Asia/Atyrau", (short)18000},
  {"Asia/Baghdad", (short)10800},
  {"Asia/Bahrain", (short)10800},
  {"Asia/Baku", (short)14400},
  {"Asia/Bangkok", (short)25200},
  {"Asia/Barnaul", (short)25200},
  {"Asia/Beirut", (short)10800},
  {"Asia/Bishkek", (short)21600},
  {"Asia/Brunei", (short)28800},
  {"Asia/Calcutta", (short)19800},
  {"Asia/Chita", (short)32400},
  {"Asia/Choibalsan", (short)28800},
  {"Asia/Chongqing", (short)28800},
  {"Asia/Chungking", (short)28800},
  {"Asia/Colombo", (short)19800},
  {"Asia/Dacca", (short)21600},
  {"Asia/Damascus", (short)10800},
  {"Asia/Dhaka", (short)21600},
  {"Asia/Dili", (short)32400},
  {"Asia/Dubai", (short)14400},
  {"Asia/Dushanbe", (short)18000},
  {"Asia/Famagusta", (short)10800},
  {"Asia/Gaza", (short)10800},
  {"Asia/Harbin", (short)28800},
  {"Asia/Hebron", (short)10800},
  {"Asia/Ho_Chi_Minh", (short)25200},
  {"Asia/Hong_Kong", (short)28800},
  {"Asia/Hovd", (short)25200},
  {"Asia/Irkutsk", (short)28800},
  {"Asia/Istanbul", (short)10800},
  {"Asia/Jakarta", (short)25200},
  {"Asia/Jayapura", (short)32400},
  {"Asia/Jerusalem", (short)10800},
  {"Asia/Kabul", (short)16200},
  {"Asia/Kamchatka", (short)43200},
  {"Asia/Karachi", (short)18000},
  {"Asia/Kashgar", (short)21600},
  {"Asia/Kathmandu", (short)20700},
  {"Asia/Katmandu", (short)20700},
  {"Asia/Khandyga", (short)32400},
  {"Asia/Kolkata", (short)19080},
  {"Asia/Krasnoyarsk", (short)25200},
  {"Asia/Kuala_Lumpur", (short)28800},
  {"Asia/Kuching", (short)28800},
  {"Asia/Kuwait", (short)10800},
  {"Asia/Macao", (short)28800},
  {"Asia/Macau", (short)28800},
  {"Asia/Magadan", (short)39600},
  {"Asia/Makassar", (short)28800},
  {"Asia/Manila", (short)28800},
  {"Asia/Muscat", (short)14400},
  {"Asia/Nicosia", (short)10800},
  {"Asia/Novokuznetsk", (short)25200},
  {"Asia/Novosibirsk", (short)25200},
  {"Asia/Omsk", (short)21600},
  {"Asia/Oral", (short)18000},
  {"Asia/Phnom_Penh", (short)25200},
  {"Asia/Pontianak", (short)25200},
  {"Asia/Pyongyang", (short)32400},
  {"Asia/Qatar", (short)10800},
  {"Asia/Qostanay", (short)21600},
  {"Asia/Qyzylorda", (short)18000},
  {"Asia/Rangoon", (short)23400},
  {"Asia/Riyadh", (short)10800},
  {"Asia/Saigon", (short)25200},
  {"Asia/Sakhalin", (short)39600},
  {"Asia/Samarkand", (short)18000},
  {"Asia/Seoul", (short)32400},
  {"Asia/Shanghai", (short)28800},
  {"Asia/Singapore", (short)28800},
  {"Asia/Srednekolymsk", (short)39600},
  {"Asia/Taipei", (short)28800},
  {"Asia/Tashkent", (short)18000},
  {"Asia/Tbilisi", (short)14400},
  {"Asia/Tehran", (short)12600},
  {"Asia/Tel_Aviv", (short)10800},
  {"Asia/Thimbu", (short)21600},
  {"Asia/Thimphu", (short)21600},
  {"Asia/Tokyo", (short)32400},
  {"Asia/Tomsk", (short)25200},
  {"Asia/Ujung_Pandang", (short)28800},
  {"Asia/Ulaanbaatar", (short)28800},
  {"Asia/Ulan_Bator", (short)28800},
  {"Asia/Urumqi", (short)21600},
  {"Asia/Ust-Nera", (short)36000},
  {"Asia/Vientiane", (short)25200},
  {"Asia/Vladivostok", (short)36000},
  {"Asia/Yakutsk", (short)32400},
  {"Asia/Yangon", (short)23400},
  {"Asia/Yekaterinburg", (short)18000},
  {"Asia/Yerevan", (short)14400},
  {"Atlantic/Azores", (short)0},
  {"Atlantic/Bermuda", (short)-10800},
  {"Atlantic/Canary", (short)3600},
  {"Atlantic/Cape_Verde", (short)-3600},
  {"Atlantic/Faeroe", (short)3600},
  {"Atlantic/Faroe", (short)3600},
  {"Atlantic/Jan_Mayen", (short)7200},
  {"Atlantic/Madeira", (short)3600},
  {"Atlantic/Reykjavik", (short)0},
  {"Atlantic/South_Georgia", (short)-7200},
  {"Atlantic/St_Helena", (short)0},
  {"Atlantic/Stanley", (short)-10800},
  {"Australia/ACT", (short)36000},
  {"Australia/Adelaide", (short)34200},
  {"Australia/Brisbane", (short)36000},
  {"Australia/Broken_Hill", (short)34200},
  {"Australia/Canberra", (short)36000},
  {"Australia/Currie", (short)36000},
  {"Australia/Darwin", (short)34200},
  {"Australia/Eucla", (short)31500},
  {"Australia/Hobart", (short)36000},
  {"Australia/LHI", (short)37800},
  {"Australia/Lindeman", (short)36000},
  {"Australia/Lord_Howe", (short)37800},
  {"Australia/Melbourne", (short)36000},
  {"Australia/NSW", (short)36000},
  {"Australia/North", (short)34200},
  {"Australia/Perth", (short)28800},
  {"Australia/Queensland", (short)36000},
  {"Australia/South", (short)34200},
  {"Australia/Sydney", (short)36000},
  {"Australia/Tasmania", (short)36000},
  {"Australia/Victoria", (short)36000},
  {"Australia/West", (short)28800},
  {"Australia/Yancowinna", (short)34200},
  {"Brazil/Acre", (short)-18000},
  {"Brazil/DeNoronha", (short)7200},
  {"Brazil/East", (short)-10800},
  {"Brazil/West", (short)-14400},
  {"Canada/Atlantic", (short)-10800},
  {"Canada/Central", (short)-18000},
  {"Canada/East-Saskatchewan", (short)-21600},
  {"Canada/Eastern", (short)-14400},
  {"Canada/Mountain", (short)-21600},
  {"Canada/Newfoundland", (short)-9000},
  {"Canada/Pacific", (short)-25200},
  {"Canada/Saskatchewan", (short)-21600},
  {"Canada/Yukon", (short)-25200},
  {"Chile/Continental", (short)-10800},
  {"Chile/EasterIsland", (short)-18000},
  {"Cuba", (short)-14400},
  {"Etc/Greenwich", (short)0},
  {"Etc/Universal", (short)0},
  {"Etc/Zulu", (short)0},
  {"Europe/Amsterdam", (short)7200},
  {"Europe/Andorra", (short)7200},
  {"Europe/Astrakhan", (short)14400},
  {"Europe/Athens", (short)10800},
  {"Europe/Belfast", (short)3600},
  {"Europe/Belgrade", (short)7200},
  {"Europe/Berlin", (short)7200},
  {"Europe/Bratislava", (short)7200},
  {"Europe/Brussels", (short)7200},
  {"Europe/Bucharest", (short)10800},
  {"Europe/Budapest", (short)7200},
  {"Europe/Busingen", (short)7200},
  {"Europe/Chisinau", (short)10800},
  {"Europe/Copenhagen", (short)7200},
  {"Europe/Dublin", (short)3600},
  {"Europe/Gibraltar", (short)7200},
  {"Europe/Guernsey", (short)3600},
  {"Europe/Helsinki", (short)10800},
  {"Europe/Isle_of_Man", (short)3600},
  {"Europe/Istanbul", (short)10800},
  {"Europe/Jersey", (short)3600},
  {"Europe/Kaliningrad", (short)7200},
  {"Europe/Kiev", (short)10800},
  {"Europe/Kirov", (short)7200},
  {"Europe/Lisbon", (short)3600},
  {"Europe/Ljubljana", (short)7200},
  {"Europe/London", (short)3600},
  {"Europe/Luxembourg", (short)7200},
  {"Europe/Madrid", (short)7200},
  {"Europe/Malta", (short)7200},
  {"Europe/Mariehamn", (short)10800},
  {"Europe/Minsk", (short)10800},
  {"Europe/Monaco", (short)7200},
  {"Europe/Moscow", (short)10800},
  {"Europe/Nicosia", (short)10800},
  {"Europe/Oslo", (short)7200},
  {"Europe/Paris", (short)7200},
  {"Europe/Podgorica", (short)7200},
  {"Europe/Prague", (short)7200},
  {"Europe/Riga", (short)10800},
  {"Europe/Rome", (short)7200},
  {"Europe/Samara", (short)14400},
  {"Europe/San_Marino", (short)7200},
  {"Europe/Sarajevo", (short)7200},
  {"Europe/Saratov", (short)14400},
  {"Europe/Simferopol", (short)10800},
  {"Europe/Skopje", (short)7200},
  {"Europe/Sofia", (short)10800},
  {"Europe/Stockholm", (short)7200},
  {"Europe/Tallinn", (short)10800},
  {"Europe/Tirane", (short)7200},
  {"Europe/Tiraspol", (short)10800},
  {"Europe/Ulyanovsk", (short)14400},
  {"Europe/Uzhgorod", (short)10800},
  {"Europe/Vaduz", (short)7200},
  {"Europe/Vatican", (short)7200},
  {"Europe/Vienna", (short)7200},
  {"Europe/Vilnius", (short)10800},
  {"Europe/Volgograd", (short)10800},
  {"Europe/Warsaw", (short)7200},
  {"Europe/Zagreb", (short)7200},
  {"Europe/Zaporozhye", (short)10800},
  {"Europe/Zurich", (short)7200},
  {"Hongkong", (short)28800},
  {"Iceland", (short)0},
  {"Indian/Antananarivo", (short)10800},
  {"Indian/Chagos", (short)21600},
  {"Indian/Christmas", (short)25200},
  {"Indian/Cocos", (short)23400},
  {"Indian/Comoro", (short)10800},
  {"Indian/Kerguelen", (short)18000},
  {"Indian/Mahe", (short)14400},
  {"Indian/Maldives", (short)18000},
  {"Indian/Mauritius", (short)14400},
  {"Indian/Mayotte", (short)10800},
  {"Indian/Reunion", (short)14400},
  {"Iran", (short)12600},
  {"Israel", (short)10800},
  {"Jamaica", (short)-18000},
  {"Japan", (short)32400},
  {"Kwajalein", (short)43200},
  {"Libya", (short)7200},
  {"Mexico/BajaNorte", (short)-25200},
  {"Mexico/BajaSur", (short)-21600},
  {"Mexico/General", (short)-18000},
  {"NET", (short)14400},
  {"NST", (short)43200},
  {"NZ", (short)43200},
  {"NZ-CHAT", (short)45900},
  {"Navajo", (short)-21600},
  {"Pacific/Apia", (short)46800},
  {"Pacific/Auckland", (short)43200},
  {"Pacific/Bougainville", (short)39600},
  {"Pacific/Chatham", (short)45900},
  {"Pacific/Chuuk", (short)36000},
  {"Pacific/Easter", (short)-18000},
  {"Pacific/Efate", (short)39600},
  {"Pacific/Enderbury", (short)46800},
  {"Pacific/Fakaofo", (short)46800},
  {"Pacific/Fiji", (short)43200},
  {"Pacific/Funafuti", (short)43200},
  {"Pacific/Galapagos", (short)-21600},
  {"Pacific/Gambier", (short)-32400},
  {"Pacific/Guadalcanal", (short)39600},
  {"Pacific/Guam", (short)36000},
  {"Pacific/Honolulu", (short)-36000},
  {"Pacific/Johnston", (short)-36000},
  {"Pacific/Kiritimati", (short)50400},
  {"Pacific/Kosrae", (short)39600},
  {"Pacific/Kwajalein", (short)43200},
  {"Pacific/Majuro", (short)43200},
  {"Pacific/Marquesas", (short)-32400},
  {"Pacific/Midway", (short)-39600},
  {"Pacific/Nauru", (short)43200},
  {"Pacific/Niue", (short)-39600},
  {"Pacific/Norfolk", (short)39600},
  {"Pacific/Noumea", (short)39600},
  {"Pacific/Pago_Pago", (short)-39600},
  {"Pacific/Palau", (short)32400},
  {"Pacific/Pitcairn", (short)-28800},
  {"Pacific/Pohnpei", (short)39600},
  {"Pacific/Ponape", (short)39600},
  {"Pacific/Port_Moresby", (short)36000},
  {"Pacific/Rarotonga", (short)-36000},
  {"Pacific/Saipan", (short)36000},
  {"Pacific/Samoa", (short)-39600},
  {"Pacific/Tahiti", (short)-36000},
  {"Pacific/Tarawa", (short)43200},
  {"Pacific/Tongatapu", (short)46800},
  {"Pacific/Truk", (short)36000},
  {"Pacific/Wake", (short)43200},
  {"Pacific/Wallis", (short)43200},
  {"Pacific/Yap", (short)36000},
  {"Poland", (short)7200},
  {"Portugal", (short)3600},
  {"ROC", (short)28800},
  {"ROK", (short)32400},
  {"SST", (short)39600},
  {"Singapore", (short)28800},
  {"Turkey", (short)10800},
  {"UCT", (short)0},
  {"US/Alaska", (short)-28800},
  {"US/Aleutian", (short)-32400},
  {"US/Arizona", (short)-25200},
  {"US/Central", (short)-18000},
  {"US/East-Indiana", (short)-14400},
  {"US/Eastern", (short)-14400},
  {"US/Hawaii", (short)-36000},
  {"US/Indiana-Starke", (short)-18000},
  {"US/Michigan", (short)-14400},
  {"US/Mountain", (short)-21600},
  {"US/Pacific", (short)-25200},
  {"US/Pacific-New", (short)-25200},
  {"US/Samoa", (short)-39600},
  {"UTC", (short)10800},
  {"Zulu", (short)0}
};

int timeOffset(const char* tz){
  int size = sizeof(_to)/sizeof(_to[0]);
  for(int i = 0; i < size; i++) {
    if (strcmp(tz, _to[i].tz)==0){
      return _to[i].offset;
    }
  }
  return 0;
}

#endif