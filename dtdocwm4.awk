BEGIN { 
  FS = "\t"; 
  printf("dtdivert(-1)\n");
  printf("changecom\n");
}
{
  printf("dxdefine(`%s_partno', `%s')\n", $4, $1);
  printf("dxdefine(`%s_title', `%s')\n", $4, $2);
  printf("dxdefine(`%s_revision', `%s')\n", $4, $3);
  printf("dxdefine(`%s_parent', `%s')\n", $4, $78);
}
END {
  printf("dxdefine(`dtdoclist', `indir($1`'_`'$2)')\n");
  printf("dxdefine(`fIdtdoclist', `fI`'dtdoclist($@)')\n");
  printf("dxdefine(`fBdtdoclist', `fB`'dtdoclist($@)')\n");
  printf("dxdefine(`s9dtdoclist', `s9`'dtdoclist($@)')\n");
  printf("dtdivert(0)");
}
