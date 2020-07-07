import { DB } from 'https://deno.land/x/sqlite/mod.ts'
import { Client } from 'https://deno.land/x/postgres/mod.ts'

/**
 * Tableau Assignments
    Data is the same as `dinoworldfun.db` but distributed as `.json` files.
    As of this writing joining tables in Tableau from JSON files is not easily supported.
 * Run with:
 * $ deno run --allow-read --allow-write --unstable --allow-net .\getData.ts
 */

interface Attraction {
  attractionId: number
  name: string
  region: string
  category: string
  attractionType: string
}
interface Visitor {
  visitorId: number
}
interface Checkin {
  id: number
  visitorId: number
  checkinTime: string
  attraction: number
  duration: string
  checkinType: string
}
interface Sequence {
  id: number
  visitorId: number
  attractionSequence: string[]
}

function createAttraction(client: Client, attractions: Attraction[]) {
  const singleInsert = (attraction: Attraction) => `(
    ${attraction.attractionId}, 
    '${attraction.name}', 
    '${attraction.region}',
    '${attraction.category}', 
    '${attraction.attractionType}'
  )`
  return client.query(`INSERT INTO attraction (AttractionID, Name, Region, Category, attractionType)
      VALUES ${attractions.map(singleInsert).join(',')}
    `)
}

function createVisitor(client: Client, visitors: Visitor[]) {
  const singleInsert = (visitor: Visitor) => `(
    ${visitor.visitorId}
  )`
  return client.query(`INSERT INTO visitor (visitorID)
      VALUES ${visitors.map(singleInsert).join(',')}
      ON CONFLICT DO NOTHING
    `)
}

function createCheckin(client: Client, checkins: Checkin[]) {
  const singleInsert = (checkin: Checkin) => `(
    ${checkin.id},
    ${checkin.visitorId},
    to_timestamp('${checkin.checkinTime}', 'yyyy-mm-dd hh24:mi:ss'),
    ${checkin.attraction},
    '${checkin.duration}',
    '${checkin.checkinType}'
  )`
  return client.query(`INSERT INTO checkin (id, visitorID, checkingTime, attraction, duration, checkinType)
      VALUES ${checkins.map(singleInsert).join(',')}
    `)
}

function createSequence(client: Client, sequences: Sequence[]) {
  const singleInsert = (sequence: Sequence) => `(
    ${sequence.id},
    ${sequence.visitorId},
    '{${sequence.attractionSequence.join(',')}}'
  )`
  return client.query(`INSERT INTO sequences (id, visitorID, attractionSequence)
      VALUES ${sequences.map(singleInsert).join(',')}
    `)
}

function Migrations() {
  const BUFFER_SIZE = 100000
  async function migrateAttractions(clientSqlite: DB, psqlClient: Client) {
    const attractions = clientSqlite.query('SELECT * FROM attraction')
    let buffer: Attraction[] = []
    for (const [
      _,
      attractionId,
      name,
      region,
      category,
      attractionType
    ] of attractions) {
      const theName = `${name}`.replace(/'/g, "''")
      if (buffer.length === BUFFER_SIZE) {
        await createAttraction(psqlClient, buffer)
        buffer = []
      }
      buffer.push({
        attractionId,
        name: theName,
        region,
        category,
        attractionType
      })
    }
    if (buffer.length != 0) {
      await createAttraction(psqlClient, buffer)
    }
  }

  async function migrateCheckins(clientSqlite: DB, psqlClient: Client) {
    const checkins = clientSqlite.query('SELECT * FROM checkin')
    let bufferVisitors: Visitor[] = []
    let bufferCheckins: Checkin[] = []
    for (const [
      id,
      visitorId,
      checkinTime,
      attraction,
      duration,
      checkinType
    ] of checkins) {
      if (bufferCheckins.length === BUFFER_SIZE) {
        if (bufferVisitors.length !== 0) {
          await createVisitor(psqlClient, bufferVisitors)
        }
        await createCheckin(psqlClient, bufferCheckins)
        bufferVisitors = []
        bufferCheckins = []
      }
      bufferVisitors.push({
        visitorId
      })
      bufferCheckins.push({
        id,
        visitorId,
        checkinTime,
        attraction,
        duration,
        checkinType
      })
    }
    if (bufferVisitors.length != 0) {
      await createVisitor(psqlClient, bufferVisitors)
    }
    if (bufferCheckins.length != 0) {
      await createCheckin(psqlClient, bufferCheckins)
    }
  }
  async function migrateSequences(clientSqlite: DB, psqlClient: Client) {
    const sequences = clientSqlite.query('SELECT * FROM sequences')
    let bufferVisitors: Visitor[] = []
    let bufferSequences: Sequence[] = []
    for (const [id, visitorId, attractionSequence] of sequences) {
      const seq = attractionSequence.split('-')
      if (bufferSequences.length === BUFFER_SIZE) {
        if (bufferVisitors.length !== 0) {
          await createVisitor(psqlClient, bufferVisitors)
        }
        await createSequence(psqlClient, bufferSequences)
      }
      bufferVisitors.push({
        visitorId
      })
      bufferSequences.push({
        id,
        visitorId,
        attractionSequence: seq
      })
    }
    if (bufferVisitors.length != 0) {
      await createVisitor(psqlClient, bufferVisitors)
    }
    if (bufferSequences.length != 0) {
      await createSequence(psqlClient, bufferSequences)
    }
  }
  return {
    migrateAttractions,
    migrateCheckins,
    migrateSequences
  }
}
async function main() {
  const sqliteClient = new DB('../readonly/dinofunworld.db')
  const psqlClient = new Client({
    user: 'postgres',
    database: 'assignment7',
    hostname: 'localhost',
    password: 'vFFvozTDxBwUdwbGnTTk',
    port: 5432
  })
  await psqlClient.connect()
  const migrate = Migrations()
  console.log('migrating attractions...')
  await migrate.migrateAttractions(sqliteClient, psqlClient)
  console.log('migrating checkins...')
  await migrate.migrateCheckins(sqliteClient, psqlClient)
  console.log('migrating sequences...')
  await migrate.migrateSequences(sqliteClient, psqlClient)
}

main()
