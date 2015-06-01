# AncientNM
Network monitoring daemon using raw socket.
Intended to run on **uClibc++** environment. Since uClibc++ does not support most C++ features, code should be limited to fit in uClibc++. See uClibc++ FAQ. [http://cxx.uclibc.org/faq.html]

In some networks, network administrators block ICMP packet. It prevents using ordinary ping to monitor network condition.
To workaround this, this project uses "stealth ping" which uses half-open TCP connection.

Project name is come from pun about acronym of Korea University. (korean acronym 고대, which means Korea University but also means Ancient. Additionally, network condition of Korea univ is aweful.)
