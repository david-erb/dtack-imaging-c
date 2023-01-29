sun4150/cast1.tim	cast with no case in inner loop, (cast.o 50K)
			performs marginally better than cast2
sun4150/cast2.tim	cast with no separate functions, (cast.o 13K)
sun4150/cast3.tim	cast with functions by source, (cast.o 22K)
			performs 10-20% better than cast2 in most cases
sun4150/cast4.tim	cast with all separate functions, (cast.o 70K)
			performs marginally better than cast3, but not in
			all cases
alpha33/qw0cast.tim	like above, all in one file