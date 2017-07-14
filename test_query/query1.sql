select d.DOSERIAL AS SERIAL,
	d.DOFLGART,
	d.DOKEYART,
	a.ARDESART as DESCRI,
	"FIXfields" AS fisso
from ba_docume001 d
	inner join ba_artmod a on d.DOFLGART=a.ARFLGART and d.DOKEYART = a.ARKEYART
WHERE a.arflgart=7 and (d.DOQTAMOV is not null or d.doqtamov <> 0)