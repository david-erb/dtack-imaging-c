# ==============================
function one(dir, product)
{
  printf("<hr>\n") >> index_html
  printf("<b>%s</b>\n", title1[product]) >> index_html
  if (title2[product] != "")
  {
    printf("<br>\n") >> index_html
    printf("&#160;&#160;%s\n", title2[product]) >> index_html
  }

  printf("<ul>\n") >> index_html
  printf("<li><a href=\"/%s/\">", product) >> index_html
    printf("Browse documentation ") >> index_html
    printf("</a>\n") >> index_html

  if (product != "dtbase")
  {
  printf("<li>Download technical documentation (Postscript 300dpi) ") >> index_html
  printf("<a href=\"%s/%s.ps3.gz\">", dir, product) >> index_html
    printf("%s.ps3.gz\n", product) >> index_html
    printf("</a>\n") >> index_html
  printf("<li>Download technical documentation (Postscript 600dpi) ") >> index_html
  printf("<a href=\"%s/%s.ps6.gz\">", dir, product) >> index_html
    printf("%s.ps6.gz\n", product) >> index_html
    printf("</a>\n") >> index_html  
  }

  printf("<li>Download source code ") >> index_html
  printf("<a href=\"%s/%s.tar.gz\">", dir, product) >> index_html
    printf("%s.tar.gz\n", product) >> index_html
    printf("</a>\n") >> index_html
  printf("<li>Download source code ") >> index_html
  printf("<a href=\"%s/%s.zip\">", dir, product) >> index_html
    printf("%s.zip\n", product) >> index_html
    printf("</a>\n") >> index_html
  printf("</ul>\n") >> index_html
}
