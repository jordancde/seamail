#!/bin/bash

python -m json.tool state.json | vim - -c"set syntax=json"
