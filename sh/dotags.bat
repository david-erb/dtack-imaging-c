del TAGS
find . -name '*.c' -exec etags --append --output=TAGS {} ;
find . -name '*.h' -exec etags --append --output=TAGS {} ;
find . -name '*.i' -exec etags --append --output=TAGS {} ;
find . -name '*.sh' -exec etags --append --output=TAGS {} ;
find . -name '*.p' -exec etags --append --output=TAGS {} ;

