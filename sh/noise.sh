set -x

#dtu gen -sigma 10 -mu 128 9 /img/gen/gauss.10.128
#dtu gen -sigma 20 -mu 128 9 /img/gen/gauss.20.128
#dtu gen -sigma 30 -mu 128 9 /img/gen/gauss.30.128
#dtu gen -sigma 40 -mu 128 9 /img/gen/gauss.40.128
#dtu gen -sigma 50 -mu 128 9 /img/gen/gauss.50.128

dtu copy /img/gen/gauss.10.128 :noise10
dtu linear :a 1 :noise10 1 -128 :b10
dtu hist :b10 :h10
dtu snr :a :b10

dtu copy /img/gen/gauss.20.128 :noise20
dtu linear :a 1 :noise20 1 -128 :b20
dtu hist :b20 :h20
dtu snr :a :b20

dtu copy /img/gen/gauss.40.128 :noise40
dtu linear :a 1 :noise40 1 -128 :b40
dtu hist :b40 :h40
dtu snr :a :b40
