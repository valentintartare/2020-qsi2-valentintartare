open Framework;
open QCheckRely;
open Generator.Fantasy;
open Lib.Troll;

let {describe} = extendDescribe(QCheckRely.Matchers.matchers);

describe("Troll Invariance", ({test}) => {
  test("Troll score should be 0 when all elves resurrected", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="Troll score should be 0 when all elves resurrected",
      troll_arbitrary,
      troll =>
      all_elves_resurrected(troll) |> scoring == 0
    )
    |> expect.ext.qCheckTest;
    ();
  });
  test("Troll score should always be >= 0", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="Troll score should always be >= 0",
      troll_arbitrary,
      troll =>
      scoring(troll) >= 0
    ) |> expect.ext.qCheckTest;
    ();
  });
});

describe("Troll Inverse", ({test}) => {
  test("oops_he_survived should always be inverse of i_got_one", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="oops_he_survived should always be inverse of i_got_one",
      troll_elf_arbitrary,
      ((troll, elf)) => {
      let scoreTrollInitial = troll
      let scoreTrollWhenHeFinallySurvived = i_got_one(elf, oops_he_survived(elf, troll));
      scoring(scoreTrollWhenHeFinallySurvived) == scoring(scoreTrollInitial);
      }
    ) |> expect.ext.qCheckTest;
    ();
  })
});

describe("Troll Analogy", ({test}) => {
  test("i_got_one and i_got should be consistent", ({expect}) => {
    QCheck.Test.make(
      ~count=1000,
      ~name="i_got_one and i_got should be consistent",
      troll_elf_int_arbitrary,
      ((troll, elf, times)) => {
        let scoreTrollWhenKilledManyElvesOneTime = ref(troll);
        for (x in 1 to times) {
          scoreTrollWhenKilledManyElvesOneTime := i_got_one(elf, scoreTrollWhenKilledManyElvesOneTime^);
        };
        let scoreTrollWhenHeFinallySurvived = i_got(times, elf, troll);
        scoring(scoreTrollWhenHeFinallySurvived) == scoring(scoreTrollWhenKilledManyElvesOneTime^);
      }
    ) |> expect.ext.qCheckTest;
    ();
  })
});

describe("Troll Idempotence", ({test}) => {
  test(
    "all_elves_of_a_kind_resurrected brings the Troll killing list to a stable state",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="all_elves_of_a_kind_resurrected brings the Troll killing list to a stable state",
        troll_arbitrary,
        troll => {
          let stableTroll = all_elves_resurrected(troll);
          scoring(stableTroll) == 0;
        }
      ) |> expect.ext.qCheckTest;
      ();
  })
});

describe("Troll Metamophism", ({test}) => {
  test(
    "i_got_one increase the Troll killing list",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="i_got_one increase the Troll killing list",
        troll_elf_arbitrary,
        ((troll, elf)) => {
          let initialTroll = troll;
          let initialTrollKilledTheElf = i_got_one(elf, troll);
          scoring(initialTroll) < scoring(initialTrollKilledTheElf);
        }
      ) |> expect.ext.qCheckTest;
      ();
  })
});

describe("Troll Injection", ({test}) => {
  test(
    "i_got_one increase the Troll killing list differently",
    ({expect}) => {
      QCheck.Test.make(
        ~count=1000,
        ~name="i_got_one increase the Troll killing list",
        troll_two_elves_arbitrary,
        ((troll, elf1, elf2)) => {
          let initialTrollkilledTheFirstElf = i_got_one(elf1, troll);
          let initialTrollKilledTheSecondElf = i_got_one(elf2, initialTrollkilledTheFirstElf);
          scoring(initialTrollkilledTheFirstElf) < scoring(initialTrollKilledTheSecondElf);
        }
      ) |> expect.ext.qCheckTest;
      ();
  })
});