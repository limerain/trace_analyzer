default:
	gcc devs_info.c main.c table.c parser.c map.c -o trace_analizer -w
clean:
	rm -rf trace_analizer
