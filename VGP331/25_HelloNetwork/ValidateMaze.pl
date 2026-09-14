use strict;
use warnings;
use JSON::PP qw(decode_json);
use FindBin;

chdir $FindBin::Bin or die "Cannot enter sample directory: $!";
sub require_true { die "$_[1]\n" unless $_[0]; }
sub read_text {
    open my $file, '<', $_[0] or die "Cannot open $_[0]: $!";
    local $/;
    return <$file>;
}
sub load_json { return decode_json(read_text($_[0])); }
sub check_textures {
    my ($mesh) = @_;
    for my $path (values %{$mesh->{Textures} // {}}) {
        require_true(-f $path, "Missing texture: $path") if length $path;
    }
}

my $level = load_json('../../Assets/Templates/Levels/level_mp.json');
require_true(exists $level->{Services}{NetworkService}, 'Missing NetworkService');
for my $entry (values %{$level->{GameObjects}}) {
    my $template = load_json($entry->{Template});
    check_textures($template->{Components}{MeshComponent});
    check_textures($entry->{Components}{MeshComponent});
}
my $local = load_json($level->{GameObjects}{Player}{Template})->{Components};
my $remote = load_json($level->{Services}{NetworkService}{PlayerTemplate})->{Components};
check_textures($remote->{MeshComponent});
require_true($local->{MeshComponent}{Textures}{DiffuseMap} ne $remote->{MeshComponent}{Textures}{DiffuseMap}, 'Players need distinct textures');
for my $player ($local, $remote) {
    my $radius = $player->{MeshComponent}{Shape}{Radius};
    require_true($radius == $player->{RigidBodyComponent}{ColliderData}{Radius}, 'Player mesh/collider radii differ');
    require_true(2 * $radius < 3.6, 'Player does not fit corridor');
}

my $source = read_text('GameState.cpp');
my ($block) = $source =~ /constexpr WallSegment MazeWalls\[\]\s*=\s*\{(.*?)\};/s;
require_true(defined $block, 'Wall layout not found');
my (%horizontal, %vertical);
my $count = 0;
while ($block =~ /\{\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+)\s*\}/g) {
    my ($x1, $y1, $x2, $y2) = ($1, $2, $3, $4);
    require_true(($x1 == $x2) != ($y1 == $y2), 'Wall must be axis aligned and nonzero');
    require_true($x2 >= $x1 && $y2 >= $y1 && $x2 <= 6 && $y2 <= 6, 'Wall outside grid');
    for (my $x = $x1; $x < $x2; ++$x) { $horizontal{"$x,$y1"} = 1; }
    for (my $y = $y1; $y < $y2; ++$y) { $vertical{"$x1,$y"} = 1; }
    ++$count;
}
require_true($count == 23, 'Expected 23 wall segments');
for my $i (0..5) {
    require_true(!!$horizontal{"$i,0"} == ($i != 2), 'Incorrect top entrance');
    require_true(!!$horizontal{"$i,6"} == ($i != 3), 'Incorrect bottom exit');
    require_true($vertical{"0,$i"} && $vertical{"6,$i"}, 'Side boundary must be closed');
}
my @queue = ([2, 0]);
my %seen = ('2,0' => 1);
while (@queue) {
    my ($x, $y) = @{shift @queue};
    for my $step ([-1,0], [1,0], [0,-1], [0,1]) {
        my ($nx, $ny) = ($x + $step->[0], $y + $step->[1]);
        next if $nx < 0 || $ny < 0 || $nx >= 6 || $ny >= 6;
        my $bx = $x > $nx ? $x : $nx;
        my $by = $y > $ny ? $y : $ny;
        my $blocked = $nx != $x ? $vertical{"$bx,$y"} : $horizontal{"$x,$by"};
        next if $blocked || $seen{"$nx,$ny"};
        $seen{"$nx,$ny"} = 1;
        push @queue, [$nx, $ny];
    }
}
require_true($seen{'3,5'}, 'Exit unreachable from entrance');
require_true(scalar(keys %seen) == 36, 'Maze has unreachable cells');
require_true($level->{Capacity} >= $count + 4, 'Insufficient world capacity');
require_true(index($source, 'mGameWorld.DebugUI(false)') >= 0, 'Save must remain disabled');
print "PASS: JSON/assets, 23 walls, two openings, all 36 cells reachable, player colliders, capacity and Save disabled.\n";
