#!/bin/bash
yes | rm update -r
mkdir update
cd update
git clone https://Gabriel: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@github.com/tramontina-gabrielromio/Interface
cd ..
yes | cp -rf update/Interface/* .
yes | rm update -r
