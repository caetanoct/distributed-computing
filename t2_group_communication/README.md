# Process Group Communication Project

## Ambient Setup

Use this command to check jgroups version.
```sh
java -jar jgroups-5.0.0.Final.jar
```

Use this command to extract files to jgroup directory.
```bash
cd jgroups
jar -xf ../jgroups-5.0.0.Final.jar
```

In two different terminals do this to test Jgroups:

|Terminal 1  | Terminal 2  |
|--|--|
| `java org.jgroups.demos.Draw` | `java org.jgroups.demos.Draw` |

This should open a draw application.