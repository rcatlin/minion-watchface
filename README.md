## Minion Watchface for Pebble Time

A minion watchface for the Pebble Time.

![Screenshot](docs/screenshot01.png)

## Development

Requires Python 3.*

### Setup
1. Create a directory called `pebble-dev`
2. Install Pebble SDK into `pebble-dev` by following http://developer.getpebble.com/sdk/install
3. Clone this project into `pebble-dev` directory
4. Enter virtualenv created during installation of Pebble SDK via `source PebbleSDK-3.1/.env/bin/activate`
5. `cd` into `minion-watchface` (clone git repo directory)
6. Begin development

The `Makefile` provides a number of important steps in the development process.

### Installing and running on an emulator

1. Run `make install-emu`

### Installing on your Watch

1. Enable development mode on your PebbleTime iOS or Android app.
2. Ensure your phone is connected to your watch
3. Modify the `install-phone` step in the `Makefile` to use the IP Address of your phone connected to your local network.
4. Run `make run-phone`