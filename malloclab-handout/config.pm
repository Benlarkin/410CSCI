# What is the name of this Lab?
$LABNAME = "malloclab";

# What is the distribution of points?
$MAXCORR = 50;   # Max correctness points
$MAXPERF = 50;   # Max performance points
$MAXSTYLE = 0;  # Max style points

# Where are the source files for the driver? 
# (override with -s)
$SRCDIR = "../src";

# Where is the handin directory? 
# (override with -d)
$HANDINDIR = "./handin";

# Where are the trace files for the driver?
$TRACEDIR = "~/410/malloclab/malloclab/traces/";

#
# corr_score - returns correctness score as a function of the
#       number of traces that were handled without any errors.
#
sub corr_score {
    my $numcorrect = $_[0]; # number of traces handled without errors 
    my $numtraces = 10;     # number of traces

    # Score is based on fraction of traces that were correct
    return ($numcorrect / $numtraces) * $MAXCORR;
}

#
# perf_score - returns performance score as a function of the
#      performance index. A solution with a performance index
#      above or equal to $threshhold gets full credit.
#
sub perf_score {
    my $perfindex = $_[0];  # performance index
    my $threshhold = 0.95;  # full credit threshhold
    my $score;

    $score = (($perfindex/100.0) + (1-$threshhold)) * $MAXPERF;
    if ($score > $MAXPERF) {
      $score = $MAXPERF;
    }

    return $score;
}
