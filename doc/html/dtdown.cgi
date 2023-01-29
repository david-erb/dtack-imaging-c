#!/bin/sh
#
# The purpose of this CGI script is to handle input from the dtdown/all.htm
# fill-out form.
# This script is a interactive means of accessing URL's in the secure 
# directory. This form exists merely as a convenience to the customer.
# If the customer knows his passcode, he can directly form and/or bookmark 
# the URL.
#
# This script takes two arguments.  
# Depending on the arguments, this script will do one of two things:
#   1) return as html the dtdown/all.htm with modifications made by sed
#      a) error message added if input is invalid
#      b) fill-out form's fields set if partial input is given
#   2) return as location the URL to the passcode subdirectory of the
#      secure directory
#
# The two arguments are:
#   1) passcode - all-numeric string assigned to the customer by Dtack
#      once license fee arrangements have been made
#   2) product - short alphanumeric string (no spaces) name of the product
#
# A URL is returned if either of the files:
#   1) $url_root/secure/$passcode/$product exists or
#   2) $url_root/secure/free/$product exists and $passcode is empty
#
# Otherwise it is an error.
#
# Error messages are issued by using sed to change a specific comment
# found in dtdown/all.htm to a message string and returning the entire
# html as html.
#
# ============================================================================
# The check function is for script debugging only.
check()
{
  echo "Content-type: text/html"
  echo
  echo "<pre>"
  if [ -r $template ]
  then
    echo "$template exists"
  else
    echo "$template does not exist"
  fi
 set
  echo "</pre>"
}

# ============================================================================
# cgi_root is where the dtack doc root is from the cgi script's point of view
cgi_root=.
cgi_secure=./secure

# url_root is where the dtack doc root is from the browser url point of view
url_root=.
url_secure=./secure

# html document which has message fields embedded as comments for sed to find
template=$cgi_root/dtdown/all.htm

# special case: free directory does not require a passcode
free="free"

# poor man's cgiparse
eval `echo $QUERY_STRING | tr '&' ';'`

#check
#exit

# ============================================================================
# neither product nor passcode given

if [ -z "$product" -a -z "$passcode" ]
then
  echo "Content-type: text/html"
  echo
  cat $template
fi

# ============================================================================
# passcode given but not product
# give URL of passcode directory if it exists, else error

if [ -z "$product" -a -n "$passcode" ]
then
  if [ -d $cgi_secure/$passcode ]
  then
    echo "Location: $url_secure/$passcode"
    echo
  else
    echo "Content-type: text/html"
    echo
    cat $template | \
    sed "s%value=\"\"%value=\"$passcode\"%" | \
    sed 's%<!-- error -->%<p><h2>Sorry, that passcode is unrecognized.</h2><p>%'
  fi
fi

# ============================================================================
# product given but passcode not given
# give URL if product is in free directory, else error

if [ -n "$product" -a -z "$passcode" ]
then
  if [ -d $cgi_secure/$free/$product ]
  then
    echo "Location: $url_secure/$free/$product"
    echo
  else
    echo "Content-type: text/html"
    echo
    cat $template | \
    sed "s%<option value=\"$product\"%<option value=\"$product\" selected%" | \
    sed 's%<!-- error -->%<p><h2>Please enter a passcode.</h2><p>%'
  fi
fi

# ============================================================================
# both product and passcode given
# give URL if product is in passcode directory, else error

if [ -n "$product" -a -n "$passcode" ]
then
  if [ -d $cgi_secure/$passcode/$product ]
  then
    echo "Location: $url_secure/$passcode/$product"
    echo
  else
  if [ "$passcode" = "ask" ]
  then
    echo "Content-type: text/html"
    echo
    cat $template | \
    sed "s%<option value=\"$product\"%<option value=\"$product\" selected%" | \
    sed 's%<!-- error -->%<p><h2>Please enter your passcode.</h2><p>%'
  else
    echo "Content-type: text/html"
    echo
    cat $template | \
    sed "s%value=\"\"%value=\"$passcode\"%" | \
    sed "s%<option value=\"$product\"%<option value=\"$product\" selected%" | \
    sed 's%<!-- error -->%<p><h2>Sorry, that passcode is unrecognized for that product.</h2><p>%'
  fi
  fi
fi

exit
