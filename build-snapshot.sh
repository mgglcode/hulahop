VERSION=0.1
ALPHATAG=`git-show-ref --hash=10 refs/heads/master`
TARBALL=hulahop-$VERSION-git$ALPHATAG.tar.bz2

rm hulahop-$VERSION.tar.bz2

make distcheck

mv hulahop-$VERSION.tar.bz2 $TARBALL
scp $TARBALL mpg@devserv.devel.redhat.com:~
rm $TARBALL
