## HEY COUNTRY, DO YOU ARE HAVE STUPID??
## Was it really that hard to find this?

# codget

[Download](https://github.com/felixbucket/codget/releases)

Blizzard Call of Duty CDN downloader. Capable of downloading upcoming versions of games. **They will most likely not be playable, but can be used for datamining and such** - if you're going to try to play the game, make sure you run it through battle.net app so it can fix the possible inconsistencies. Note that it always stores the CASC archive in the /Data folder, which is not correct for every game - you might want to rename the folder to what the actual game uses. Currently I only tested it for Diablo III.

The program represents a simple 4-step wizard.

Step 1: select game and region.  
Step 2: select build version.  
Step 3: select a combination of download tags - these are used to specify the platform, language, etc.  
Step 4: select download location and start the download.

Note that the program creates a cache in its local directory, which significantly increases disk usage (doubles or even triples, because it often loads partial archives and reserves space for the entire file), but re-downloading the game, or downloading future versions can take significantly less time. You can delete the cache folder after downloading if you value disk space over future network traffic.

Using this is not recommended for playable versions, as it is probably slower than the battle.net launcher (due to only using one download thread), and might not install everything correctly. It is also incapable of patching (aka upgrading a previous installation). You might be able to use it to pre-download an upcoming game and then use the battle.net launcher to 'fix' it (make sure to rename the Data folder to what is appropriate for the specific game, though.


Original code for BlizzGet can be found at https://github.com/d07RiV/blizzget/
