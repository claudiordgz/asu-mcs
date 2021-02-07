
Function Invoke-Clingo {
  [CmdletBinding()]
  param(
      [Int]
      [Alias("e")]
      [Parameter(Mandatory=$true, HelpMessage="Example to Run.")]
      $Example,
      [Parameter(Mandatory=$false)]
      [Alias("d")]
      [ValidateScript({Test-Path $_})]
      [string]
      $Directory
  )
  begin {
    if([string]::IsNullOrWhiteSpace($Directory))
    {
      $Directory = (Get-Item .).FullName
    }
  }
  process {
    Try {
      if ($Example.Equals(0)) {
        Get-ChildItem -Path $Directory -file *.lp | ForEach-Object {
          clingo "$([System.IO.Path]::GetFileName($_))" 0
        }
      } else {
        $file = Join-Path -Path $Directory -ChildPath "$Example.lp"
        if (Test-Path $file -PathType leaf)
        {
          clingo $file 0
        } else {
          Write-Error "$file - Does not exist"
        }
      }
    } Catch {
      Write-Error "$($_.Exception.Message) - Line Number: $($_.InvocationInfo.ScriptLineNumber)"
      throw
    }
  }
}