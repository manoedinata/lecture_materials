# AWK

## tidak perlu apa-apa di BEGIN
BEGIN {}

## tiap line, increment variable total
{
    total++;
}

## klo udh kelar, print
END {
    print "Jumlah transaksi: " total
}
