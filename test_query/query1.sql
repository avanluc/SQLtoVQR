SELECT arcodart,
       ardesart,
       arflgart
From ba_artmod a inner join ba_docume001 ba_docume on DOFLGART=ARFLGART
	left outer join ba_docume_m001 ba_docume_m on DOSERIAL=BASERIAL
WHERE arflgart=7