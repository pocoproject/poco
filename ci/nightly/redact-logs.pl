# Replaces the machine's network details in the nightly logs before they are uploaded: its own
# addresses and interface names (NIGHTLY_ADDRESSES, NIGHTLY_INTERFACES), every MAC address,
# every IPv6 address but ::1, and private, link-local and shared IPv4 addresses.
# Usage: perl -pi redact-logs.pl FILE...
BEGIN {
	@addrs = sort { length($b) <=> length($a) } split ' ', ($ENV{NIGHTLY_ADDRESSES} // '');
	@names = sort { length($b) <=> length($a) } split ' ', ($ENV{NIGHTLY_INTERFACES} // '');
}
for my $a (@addrs) { s/\Q$a\E/[address]/gi; }
for my $n (@names) { s/(?<![\w-])\Q$n\E(?![\w-])/[interface]/g; }
s/(?<![0-9a-f])(?:[0-9a-f]{2}[:-]){5}[0-9a-f]{2}(?![0-9a-f])/[mac]/gi;
s{(?<![\w:.])(?:[0-9a-f]{0,4}:){2,7}[0-9a-f]{0,4}(?![\w:])}{ my $m = $&; ($m ne '::1' && $m =~ /[0-9a-f]/i && ($m =~ /::/ || ($m =~ tr/://) >= 4)) ? '[ipv6]' : $m }gie;
s{(?<![\w.])(\d{1,3})\.(\d{1,3})\.\d{1,3}\.\d{1,3}(?![\w.])}{ ($1 == 10 || ($1 == 172 && $2 >= 16 && $2 <= 31) || ($1 == 192 && $2 == 168) || ($1 == 169 && $2 == 254) || ($1 == 100 && $2 >= 64 && $2 <= 127)) ? '[ipv4]' : $& }ge;
