var sio = require("socket.io")();
var ncc = require("node-netcat").client;

var app = require('express')();
var client = ncc(4000, process.argv[2]);
var server = require("http").Server(app);

var angles = [ ];
var beacons = [
    [ 1, 2, Math.PI ],
    [ 1, 0, 0 ]
];
function compute(data) {
    var parts = data.toString("ascii").split(/ +/);
    var ch = parseInt(parts[0]);
    var beacon = parseInt(parts[1]);
    var angle = (parseInt(parts[2]) / 360.0 * 135.0 + 22.5) / 180.0 * Math.PI;
    angles[ch] = [ angle, beacon ];
    console.log(parseInt(parts[2]) / 360.0 * 135.0 + 22.5);

    // Recalculate position
    //      (1, 2)
    // +----- 0 -----+
    // |             |
    // |             |
    // |             |
    // |             |
    // |             |
    // +----- 1 -----+
    //      (1, 0)

    // Generate rays
    var dx = angles.map(function(a) {
        return Math.cos(a[0] + ((a[1] >= 0) ? beacons[a[1]][2] : 0));
    });
    var dy = angles.map(function(a) {
        return Math.sin(a[0] + ((a[1] >= 0) ? beacons[a[1]][2] : 0));
    });
    var bs = angles.map(function(a) { return a[1]; });
    var x = 0;
    var y = 0;
    var l = dx.length;
    var count = 0;
    // Take pairs of rays
    for (var i = 0; i < l; i++) {
        for (var j = i + 1; j < l; j++) {
            if (bs[i] >= 0 && bs[j] >= 0) {
                var diffx = beacons[bs[j]][0] - beacons[bs[i]][0];
                var diffy = beacons[bs[j]][1] - beacons[bs[i]][1];
                var cross1 = dx[i] * dy[j] - dy[i] * dx[j];
                var cross2 = diffx * dy[j] - diffy * dx[j];
                var cross3 = diffx * dy[i] - diffy * dx[i];
                var t = cross2 / cross1;
                var u = cross3 / cross1;
                if (-1e-4 < cross1 && cross1 < 1e-4) { // Parallel
                    if (-1e-4 < cross2 && cross2 < 1e-4) { // Colinear
                        if (t > u) {
                            x += beacons[bs[i]][0] + dx[i] * t;
                            y += beacons[bs[i]][1] + dy[i] * t;
                        }
                        else {
                            x += beacons[bs[j]][0] + dx[j] * t;
                            y += beacons[bs[j]][1] + dy[j] * t;
                        }
                        count++;
                    }
                    else { // Do not intersect =|
                        // Explode
                    }
                }
                else {
                    if (t >= 0 && u >= 0) {
                        x += beacons[bs[i]][0] + dx[i] * t;
                        y += beacons[bs[i]][1] + dy[i] * t;
                        count++;
                    }
                }
            }
        }
    }
    if (count == 0) {
        x = 0;
        y = 0;
    }
    else {
        x = x / count;
        y = y / count;
    }

    console.log({ x: x, y: y });
}

compute("0 0 125 64");
compute("7 1 165 66");
