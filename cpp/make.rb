S={
  "v7" => "arm-linux-gnueabihf-g++ -static -march=armv7+fp",
  "v7a-neon" => "arm-linux-gnueabihf-g++ -static -march=armv7-a+neon",
  "v8" => "arm-linux-gnueabihf-g++ -static -march=armv8.1-a+simd",
  "arm64" => "g++ -static -march=armv8.1-a+simd",
  "arm64clang" => "clang++ -static -march=armv8.1-a+simd",
}

OPTS = "-std=c++17 -O2 -Ofast -s "

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
