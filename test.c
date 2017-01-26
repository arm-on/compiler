10 let x=7
20 gosub 100
30 let x=9
40 gosub 100
50 goto 200
100 print x, x*x
110 return
200 end