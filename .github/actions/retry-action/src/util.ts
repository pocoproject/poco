import { debug } from '@actions/core';

export async function wait(ms: number) {
  return new Promise((r) => setTimeout(r, ms));
}

export async function retryWait(retryWaitSeconds: number) {
  const waitStart = Date.now();
  await wait(retryWaitSeconds);
  debug(`Waited ${Date.now() - waitStart}ms`);
  debug(`Configured wait: ${retryWaitSeconds}ms`);
}