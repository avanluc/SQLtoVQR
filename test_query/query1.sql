SELECT arcodart,
       ardesart,
       arflgart
From ba_artmod inner join ba_docume001 on DOFLGART=ARFLGART
WHERE arflgart=7