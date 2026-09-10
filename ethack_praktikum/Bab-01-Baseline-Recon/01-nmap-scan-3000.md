```
┌──(kali㉿kali)-[~]
└─$ nmap -sV -p 3000 192.168.56.1
Starting Nmap 7.98 ( https://nmap.org ) at 2026-09-04 04:57 -0400
mass_dns: warning: Unable to determine any DNS servers. Reverse DNS is disabled. Try using --system-dns or specify valid servers with --dns-servers
Nmap scan report for 192.168.56.1
Host is up (0.00045s latency).

PORT     STATE SERVICE VERSION
3000/tcp open  ppp?
1 service unrecognized despite returning data. If you know the service/version, please submit the following fingerprint at https://nmap.org/cgi-bin/submit.cgi?new-service :
SF-Port3000-TCP:V=7.98%I=7%D=9/4%Time=6A9A881F%P=x86_64-pc-linux-gnu%r(Get
SF:Request,2668,"HTTP/1\.1\x20200\x20OK\r\nAccess-Control-Allow-Origin:\x2
SF:0\*\r\nX-Content-Type-Options:\x20nosniff\r\nX-Frame-Options:\x20SAMEOR
SF:IGIN\r\nFeature-Policy:\x20payment\x20'self'\r\nX-Recruiting:\x20/#/job
SF:s\r\nAccept-Ranges:\x20bytes\r\nCache-Control:\x20public,\x20max-age=0\
SF:r\nLast-Modified:\x20Fri,\x2004\x20Sep\x202026\x2008:02:42\x20GMT\r\nET
SF:ag:\x20W/\"24b1-1a06b71003e\"\r\nContent-Type:\x20text/html;\x20charset
SF:=UTF-8\r\nContent-Length:\x209393\r\nVary:\x20Accept-Encoding\r\nDate:\
SF:x20Fri,\x2004\x20Sep\x202026\x2008:58:03\x20GMT\r\nConnection:\x20close
SF:\r\n\r\n<!--\n\x20\x20~\x20Copyright\x20\(c\)\x202014-2026\x20Bjoern\x2
SF:0Kimminich\x20&\x20the\x20OWASP\x20Juice\x20Shop\x20contributors\.\n\x2
SF:0\x20~\x20SPDX-License-Identifier:\x20MIT\n\x20\x20-->\n\n<!doctype\x20
SF:html>\n<html\x20lang=\"en\"\x20data-beasties-container>\n<head>\n\x20\x
SF:20<meta\x20charset=\"utf-8\">\n\x20\x20<title>OWASP\x20Juice\x20Shop</t
SF:itle>\n\x20\x20<meta\x20name=\"description\"\x20content=\"Probably\x20t
SF:he\x20most\x20modern\x20and\x20sophisticated\x20insecure\x20web\x20appl
SF:ication\">\n\x20\x20<meta\x20name=\"viewport\"\x20content=\"width=devic
SF:e-width,\x20initial-scale=1\">\n\x20\x20<link\x20rel=\"preconnect\"\x20
SF:href=\"htt")%r(Help,2F,"HTTP/1\.1\x20400\x20Bad\x20Request\r\nConnectio
SF:n:\x20close\r\n\r\n")%r(NCP,2F,"HTTP/1\.1\x20400\x20Bad\x20Request\r\nC
SF:onnection:\x20close\r\n\r\n")%r(HTTPOptions,EA,"HTTP/1\.1\x20204\x20No\
SF:x20Content\r\nAccess-Control-Allow-Origin:\x20\*\r\nAccess-Control-Allo
SF:w-Methods:\x20GET,HEAD,PUT,PATCH,POST,DELETE\r\nVary:\x20Access-Control
SF:-Request-Headers\r\nContent-Length:\x200\r\nDate:\x20Fri,\x2004\x20Sep\
SF:x202026\x2008:58:03\x20GMT\r\nConnection:\x20close\r\n\r\n")%r(RTSPRequ
SF:est,EA,"HTTP/1\.1\x20204\x20No\x20Content\r\nAccess-Control-Allow-Origi
SF:n:\x20\*\r\nAccess-Control-Allow-Methods:\x20GET,HEAD,PUT,PATCH,POST,DE
SF:LETE\r\nVary:\x20Access-Control-Request-Headers\r\nContent-Length:\x200
SF:\r\nDate:\x20Fri,\x2004\x20Sep\x202026\x2008:58:03\x20GMT\r\nConnection
SF::\x20close\r\n\r\n")%r(RPCCheck,2F,"HTTP/1\.1\x20400\x20Bad\x20Request\
SF:r\nConnection:\x20close\r\n\r\n");
MAC Address: 0A:00:27:00:00:00 (Unknown)

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 11.59 seconds
                                                                             
┌──(kali㉿kali)-[~]
└─$ 
```