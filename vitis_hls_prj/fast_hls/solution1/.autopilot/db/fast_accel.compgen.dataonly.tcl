# This script segment is generated automatically by AutoPilot

set axilite_register_dict [dict create]
set port_CTRL {
threshold { 
	dir I
	width 32
	depth 1
	mode ap_none
	offset 16
	offset_end 23
}
rows { 
	dir I
	width 32
	depth 1
	mode ap_none
	offset 24
	offset_end 31
}
cols { 
	dir I
	width 32
	depth 1
	mode ap_none
	offset 32
	offset_end 39
}
ap_start { }
ap_done { }
ap_ready { }
ap_idle { }
}
dict set axilite_register_dict CTRL $port_CTRL


set port_control {
img_in { 
	dir I
	width 64
	depth 1
	mode ap_none
	offset 16
	offset_end 27
}
img_out { 
	dir I
	width 64
	depth 1
	mode ap_none
	offset 28
	offset_end 39
}
}
dict set axilite_register_dict control $port_control


