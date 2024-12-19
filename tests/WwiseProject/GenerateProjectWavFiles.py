"""
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the 
"Apache License"); you may not use this file except in compliance with the 
Apache License. You may obtain a copy of the Apache License at 
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.
"""

import os
import struct
import wave
import numpy
import random
from os import path
import argparse

dir_path = path.dirname(path.realpath(__file__))
forceRegeneration = False

def generate_wav(file_path, frame_count=500):
    sample_rate = 44100.0
    frequency = 220.0
    write_wav(file_path, sample_rate, frequency, frame_count)


def write_wav(file_path, sample_rate, frequency, frame_count):
    if forceRegeneration or not path.exists(file_path):
        print("Generating wav file : {}".format(file_path))
        wav_file = wave.open((file_path),'w')
        wav_file.setnchannels(1)
        wav_file.setsampwidth(2)
        wav_file.setframerate(sample_rate)
        
        samples = numpy.arange(frame_count) / sample_rate
        signal = numpy.sin(2.0 * numpy.pi * frequency * samples)
        signal = numpy.int16(signal * 32767.0)
        print("  writing file...")
        wav_file.writeframesraw(signal)
        wav_file.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", '--regenerate', action='store_true', help="Will force a regeneration of all files")
    args = vars(parser.parse_args())
    if args['regenerate']:
        forceRegeneration = True

    large_wav_path = path.join(dir_path, "Originals", "SFX", "LargeMedia.wav")
    generate_wav(large_wav_path, 500000000)

    for i in range(10):
        file_name = "ShortMedia_{}.wav".format(i)
        file_path = path.join(dir_path, "Originals", "SFX", file_name)
        generate_wav(file_path)
        file_name = "ShortMedia_streamed_{}.wav".format(i)
        file_path = path.join(dir_path, "Originals", "SFX", file_name)
        generate_wav(file_path)