
// Author     KMS - Martin Dubois, P.Eng.
// Copyrights (C) 2020 KMS. All rights reserved.
// Product    NetworkHelper
// File       NHLib/Error.h

#pragma once

// Constants
/////////////////////////////////////////////////////////////////////////////

#define ERROR_ERROR ("ERROR")

#define ERROR_001     ERROR_ERROR, (-1)
#define ERROR_001_FMT ("File %s - %u lines with error")

#define ERROR_002 (-2)
#define ERROR_003 (-3)
#define ERROR_004 (-4)
#define ERROR_005 (-5)
#define ERROR_006 (-6)
#define ERROR_007 (-7)

// ===== CALLER ERROR =======================================================

#define ERROR_CALLER "CALLER ERROR"

// ===== CONFIG ERROR =======================================================

#define ERROR_CONFIG ("CONFIG ERROR")

#define ERROR_201 ERROR_CONFIG, (-201), ("A VLAN number must be bellow 4096")
#define ERROR_202 ERROR_CONFIG, (-202), ("A VLAN must be set on sub-interface")
#define ERROR_203 ERROR_CONFIG, (-203), ("A same interface cannot be NAT outside and NAT inside")
#define ERROR_204 ERROR_CONFIG, (-204), ("A same interface cannot be NAT inside and NAT outside")
#define ERROR_205 ERROR_CONFIG, (-205), ("Act as DHCP server but DHCP request are not allowed in")
#define ERROR_206 ERROR_CONFIG, (-206), ("describes traffic going into the interface and the access list is used as \"out\"")
#define ERROR_207 ERROR_CONFIG, (-207), ("describe traffic going out from the interface and the access list is used as \"in\"")
#define ERROR_208 ERROR_CONFIG, (-208), ("describe opposed traffics")
#define ERROR_209 ERROR_CONFIG, (-209), ("An access list is already set for this direction")
#define ERROR_210 ERROR_CONFIG, (-210), ("Invalid IPv4 address")
#define ERROR_211 ERROR_CONFIG, (-211), ("The subnet address is not valid")
#define ERROR_212 ERROR_CONFIG, (-212), ("The subnet address is not a valid subnet address")
#define ERROR_213 ERROR_CONFIG, (-213), ("Invalid port number")
#define ERROR_214 ERROR_CONFIG, (-214), ("Only TCP rule can use the \"established\" flag")
#define ERROR_215 ERROR_CONFIG, (-215), ("Describes traffic not going through the router")
#define ERROR_216 ERROR_CONFIG, (-216), ("Describes traffic not going through the router")
#define ERROR_217 ERROR_CONFIG, (-217), ("Describes traffic not going through the network")
#define ERROR_218 ERROR_CONFIG, (-218), ("Do not create sub-interface for an interface with the DHCP client enabled")
#define ERROR_219 ERROR_CONFIG, (-219), ("Do not enable DHCP client on an interface with sub-interfaces")
#define ERROR_220 ERROR_CONFIG, (-220), ("Do not configure an IPv4 address on an interface with sub-interfaces")
#define ERROR_221 ERROR_CONFIG, (-221), ("Do not set a VLAN for an interface with sub-interfaces")
#define ERROR_222 ERROR_CONFIG, (-222), ("Do not create sub-interface for an interface with an IPv4 address configured")
#define ERROR_223 ERROR_CONFIG, (-223), ("Do not enable DHCP client on an interface with a valid IPv4 address")
#define ERROR_224 ERROR_CONFIG, (-224), ("Do not configure an IPv4 address on an interace with the DHCP client enabled")

#define ERROR_225     ERROR_CONFIG, (-225)
#define ERROR_225_FMT "\"%s\" command outside of an \"%s\" section"

#define ERROR_226 ERROR_CONFIG, (-226), ("Invalid IPv4 address")
#define ERROR_227 ERROR_CONFIG, (-227), ("Invalid IPv4 address")
#define ERROR_228 ERROR_CONFIG, (-228), ("Invalid next router IPv4 address")
#define ERROR_229 ERROR_CONFIG, (-229), ("IP routing is not enabled, and the router use more than one interface")
#define ERROR_230 ERROR_CONFIG, (-230), ("The access rule source subnet is not private")
#define ERROR_231 ERROR_CONFIG, (-231), ("The access rule source is not a private address")
#define ERROR_232 ERROR_CONFIG, (-232), ("No access rule in the access list")
#define ERROR_233 ERROR_CONFIG, (-233), ("Not used")

// ===== FILE ERROR =========================================================

#define ERROR_FILE ("FILE ERROR")

// ===== PARSE ERROR ========================================================

#define ERROR_PARSE ("PARSE ERROR")

#define ERROR_401     ERROR_PARSE, (-401)
#define ERROR_401_FMT "The command \"%s\" must have at least %u elements"

#define ERROR_402     ERROR_PARSE, (-402)
#define ERROR_402_FMT "The command \"%s\" must have %u elements or less"

#define ERROR_403 ERROR_PARSE, (-403), ("Invalid VLAN number")
#define ERROR_404 ERROR_PARSE, (-404), ("Invalid IPv4 address format")
#define ERROR_405 ERROR_PARSE, (-405), ("Invalid IPv4 inverted net mask format")

// ===== WARNING ============================================================

#define ERROR_WARNING ("WARNING")

#define ERROR_501 ERROR_WARNING, (-501), ("Disabled but configured using IPv4 static address")
