//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Tue Jan  8 10:08:15 PST 2002
// Last Modified: Mon Feb  9 21:24:41 PST 2015 Updated for C++11.
// Filename:      ...sig/doc/examples/all/createmidifile/createmidifile.cpp
// Syntax:        C++
//
// Description:   Demonstration of how to create a Multi-track MIDI file.
//

#include "json.hpp"
#include "MidiFile.h"
#include <iostream>
#include <string>

using nlohmann::json;

using namespace std;

typedef unsigned char uchar;

//string const sbjson = "/home/developer/edu/music/json2midi/part1.json";

///////////////////////////////////////////////////////////////////////////

namespace P2Midi {

    class MidiNote {
    public:
        MidiNote( ) {
            pitch                  = 0;
            duration               = 0.0;
            mean                   = 0.0;
            stablePitchRegionEnd   = 0.0;
            stablePitchRegionStart = 0.0;
            start                  = 0.0;
            type                   = "pitched";
            velocity               = 0;
        }

        MidiNote(json const & j ) {
            std::string::size_type sz;
            pitch                  = stoi (  j.at( "pitch"                  ).get<string>(), &sz );
            duration               = stold(  j.at( "duration"               ).get<string>(), &sz );
            mean                   = stold(  j.at( "mean"                   ).get<string>(), &sz );
            stablePitchRegionEnd   = stold(  j.at( "stablePitchRegionEnd"   ).get<string>(), &sz );
            stablePitchRegionStart = stold(  j.at( "stablePitchRegionStart" ).get<string>(), &sz );
            start                  = stold(  j.at( "start"                  ).get<string>(), &sz );
            type                   =         j.at( "type"                   ).get<string>();
            velocity               = stoi (  j.at( "velocity"               ).get<string>(), &sz );
        }

        int         pitch                  ; // = 0;
        long double duration               ; // = 0.0;
        long double mean                   ; // = 0.0;
        long double stablePitchRegionEnd   ; // = 0.0;
        long double stablePitchRegionStart ; // = 0.0;
        long double start                  ; // = 0.0;
        string      type                   ; // = "pitched";
        int         velocity               ; // = 0;

    };
    void to_json(json& j, const MidiNote& p) {
        j = json{
            { "pitch"                  , p.pitch                  },
            { "duration"               , p.duration               },
            { "mean"                   , p.mean                   },
            { "stablePitchRegionEnd"   , p.stablePitchRegionEnd   },
            { "stablePitchRegionStart" , p.stablePitchRegionStart },
            { "start"                  , p.start                  },
            { "type"                   , p.type                   },
            { "velocity"               , p.velocity               }
        };
    }

    void from_json(const json& j, MidiNote& p) {
        std::string::size_type sz;
        p.pitch                  = stoi( j.at( "pitch"                  ).get<string>(), &sz );
        p.duration               = stold( j.at( "duration"               ).get<string>(), &sz );
        p.mean                   = stold( j.at( "mean"                   ).get<string>(), &sz );
        p.stablePitchRegionEnd   = stold( j.at( "stablePitchRegionEnd"   ).get<string>(), &sz );
        p.stablePitchRegionStart = stold( j.at( "stablePitchRegionStart" ).get<string>(), &sz );
        p.start                  = stold( j.at( "start"                  ).get<string>(), &sz );
        p.type                   =       j.at( "type"                   ).get<string>();
        p.velocity               = stoi( j.at( "velocity"               ).get<string>(), &sz );
    }


    struct EventPairInfo {
        int pitch;
        int on_time;
        int off_time;
        int velocity;
    };
//    EventPairInfo const midinote2Messages(
//            int const ticks_per_beat,
//            int const midi_tempo,
//            MidiNote const & left,
//            MidiNote const & right
//    )
//    {
//        int const pitch                  = right.pitch;
//        long double const delta_start    = right.stablePitchRegionStart - left.stablePitchRegionEnd;
//        long double const duration_in_seconds  = right.duration;
//        int const velocity               = right.velocity;
//        int start_time_in_ticks    = int( mido.second2tick( delta_start,         ticks_per_beat, midi_tempo ) )
//        int duration_time_in_ticks = int(
//                    mido.second2tick(duration_in_seconds, ticks_per_beat, midi_tempo))// read a JSON file
//            msgs = []
//            std::ifstream ifstream1("part1.json");
//            msgs.append(mido.Message('note_on', note = pitch, velocity = velocity, time = start_time_in_ticks))
//            json read_data;
//            msgs.append(mido.Message('note_off', note = pitch, velocity = velocity, time = duration_time_in_ticks))
//            ifstream1 >> read_data;
//
//            return msgs
//            # return midinote2Messagesjson vmd;
//    }





} // namespace P2Midi

int main(int argc, char** argv) {
    // read a JSON file
    std::ifstream ifstream1("part1.json");
    json read_data;
    ifstream1 >> read_data;


    json vmd;
    vmd = read_data["vocalMetadata"];
    std::string::size_type sz;

    string vmd_tempo = vmd["tempo"];
    auto song_tempo = stof( vmd_tempo, &sz );

    cout << song_tempo;

    auto   const vocalNotes          = vmd["vocalNotes"];
    vector<P2Midi::MidiNote> const midinotes           = vocalNotes[0]["MidiNote"];
    auto   fake_first_midinote = midinotes[0];

    P2Midi::MidiNote midiNote = fake_first_midinote;


    cout << std::setprecision(18) << " duration = " << midiNote.duration;
//    cout << fake_first_midinote;

    fake_first_midinote.stablePitchRegionStart = 0.0;
    fake_first_midinote.stablePitchRegionEnd   = 0.0;

    cout << midinotes.size();

    MidiFile outputfile;        // create an empty MIDI file with one track
    vector<uchar> midievent;     // temporary storage for MIDI events
    midievent.resize(3);        // set the size of the array to 3 bytes
    int tpq = 48; // 120;              // default value in MIDI file is 48
//    float beats_per_second = song_tempo / 60.0 ;


    outputfile.setTicksPerQuarterNote(tpq);

//    int i=0;
    int actiontime = 0;      // temporary storage for MIDI event time
    int const trackId = 0;
    for ( P2Midi::MidiNote const & midiNote : midinotes )
    {
        midievent[0] = 0x90;     // store a note on command (MIDI channel 1)
        midievent[1] = static_cast<uchar>( midiNote.pitch    );
        midievent[2] = static_cast<uchar>( midiNote.velocity );
        actiontime   = static_cast<int>( midiNote.stablePitchRegionStart * tpq ); // absolute ticks
        outputfile.addEvent(trackId, actiontime, midievent);
        actiontime   = static_cast<int>( midiNote.stablePitchRegionEnd * tpq );
        midievent[0] = 0x80;     // store a note on command (MIDI channel 1)
        outputfile.addEvent(trackId, actiontime, midievent);
//        i++;
    }

    outputfile.sortTracks();         // make sure data is in correct order
    outputfile.write("swiftVocal3.mid"); // write Standard MIDI File twinkle.mid
    return 0;
}


/*  FUNCTIONS available in the MidiFile class:

void absoluteTime(void);
   Set the time information to absolute time.
int addEvent(int aTrack, int aTime, vector<uchar>& midiData);
   Add an event to the end of a MIDI track.
int addTrack(void);
   Add an empty track to the MIDI file.
int addTrack(int count);
   Add a number of empty tracks to the MIDI file.
void deltaTime(void);
   Set the time information to delta time.
void deleteTrack(int aTrack);
   remove a track from the MIDI file.
void erase(void);
   Empty the contents of the MIDI file, leaving one track with no data.
MFEvent& getEvent(int aTrack, int anIndex);
   Return a MIDI event from the Track.
int getTimeState(void);
   Indicates if the timestate is TIME_STATE_ABSOLUTE or TIME_STATE_DELTA.
int getTrackState(void);
   Indicates if the tracks are being processed as multiple tracks or
   as a single track.
int getTicksPerQuarterNote(void);
   Returns the ticks per quarter note value from the MIDI file.
int getTrackCount(void);
   Returns the number of tracks in the MIDI file.
int getNumTracks(void);
   Alias for getTrackCount();
int getNumEvents(int aTrack);
   Return the number of events present in the given track.
void joinTracks(void);
   Merge all tracks together into one track.  This function is reversable,
   unlike mergeTracks().
void mergeTracks(int aTrack1, int aTrack2);
   Combine the two tracks into a single track stored in Track1.  Track2
   is deleted.
int read(char* aFile);
   Read the contents of a MIDI file into the MidiFile class data structure
void setTicksPerQuarterNote    (int ticks);
   Set the MIDI file's ticks per quarter note information
void sortTrack(vector<MFEvent>& trackData);
   If in absolute time, sort particular track into correct time order.
void sortTracks(void);
   If in absolute time, sort tracks into correct time order.

*/



