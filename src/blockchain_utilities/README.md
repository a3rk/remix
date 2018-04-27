# Monero Blockchain Utilities

Copyright (c) 2018, The Epoch Project

## Introduction

The blockchain utilities allow one to import and export the blockchain.

## Usage:

See also each utility's "--help" option.

### Export an existing blockchain database

`$ epoch-blockchain-export`

This loads the existing blockchain and exports it to `$EPOCH_DATA_DIR/export/blockchain.raw`

### Import the exported file

`$ epoch-blockchain-import`

This imports blocks from `$EPOCH_DATA_DIR/export/blockchain.raw` (exported using the
`epoch-blockchain-export` tool as described above) into the current database.

Defaults: `--batch on`, `--batch size 20000`, `--verify on`

Batch size refers to number of blocks and can be adjusted for performance based on available RAM.

Verification should only be turned off if importing from a trusted blockchain.

If you encounter an error like "resizing not supported in batch mode", you can just re-run
the `epoch-blockchain-import` command again, and it will restart from where it left off.

```bash
## use default settings to import blockchain.raw into database
$ epoch-blockchain-import

## fast import with large batch size, database mode "fastest", verification off
$ epoch-blockchain-import --batch-size 20000 --database lmdb#fastest --verify off

```

### Import options

`--input-file`
specifies input file path for importing

default: `<data-dir>/export/blockchain.raw`

`--output-file`
specifies output file path to export to

default: `<data-dir>/export/blockchain.raw`

`--block-stop`
stop at block number

`--database <database type>`

`--database <database type>#<flag(s)>`

database type: `lmdb, memory`

flags:

The flag after the # is interpreted as a composite mode/flag if there's only
one (no comma separated arguments).

The composite mode represents multiple DB flags and support different database types:

`safe, fast, fastest`

Database-specific flags can be set instead.

LMDB flags (more than one may be specified):

`nosync, nometasync, writemap, mapasync, nordahead`

## Examples:

```
$ epoch-blockchain-import --database lmdb#fastest

$ epoch-blockchain-import --database lmdb#nosync

$ epoch-blockchain-import --database lmdb#nosync,nometasync
```
