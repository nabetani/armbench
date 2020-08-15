S={
  "v7" => "arm-linux-gnueabihf-g++ -march=armv7+fp",
  "v7a-neon2" => "arm-linux-gnueabihf-g++ -march=armv7-a+neon-vfpv4",
  "v8" => "arm-linux-gnueabihf-g++ -march=armv8-a+simd",
  "arm64" => "g++ -march=armv8-a+simd",
  "arm64clang" => "clang++ -march=armv8-a+simd",
}

OPTS = "-std=c++17 -O2 -Ofast -s -static"

File.open( "makefile", "w" ) do |f|
  f.puts( <<~"MAKEFILE" )
    all : #{S.keys.map{ |e| "bin/#{e}" }.join(" ")}

  MAKEFILE
  S.each do |k, fmt|
    f.puts( <<~"BINS" )
    bin/#{k} : make.rb main.cpp
    \tmkdir -p bin
    \t#{fmt} #{OPTS} main.cpp -latomic -lpthread -o bin/#{k}

    BINS
  end
end

File.open( "run.sh", "w" ) do |f|
  f.puts( "#!/usr/bin/bash -eu" )
  S.keys.each do |k|
    f.puts( "bin/#{k} > #{k}.csv" )
  end
end
